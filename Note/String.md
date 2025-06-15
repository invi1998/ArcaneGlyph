好的，我们来深入探讨一下 Unreal Engine 5 (UE5) 中的字符串数据类型。理解 `FString`, `FText`, `FName` 以及 `TEXT` 宏的差异、用途和最佳实践对于编写高效、可维护且支持本地化的代码至关重要。它们各有侧重，没有绝对的"最好"，只有"最适合"当前场景。

**核心概念：设计哲学**

UE 设计这些不同的字符串类型是为了解决不同的问题：

1.  **文本展示与本地化 (`FText`)：** 面向最终用户显示的文本。核心价值在于无缝支持多语言翻译、格式化和区域性（Culture）敏感的操作（如数字、日期格式）。不可变（Immutable），强调"意义"而非"字符序列"。
2.  **高性能标识与比较 (`FName`)：** 用于对象的名称、属性名、标签、资源路径等需要快速查找、比较和散列的标识符。大小写不敏感，内部存储为索引，极度高效。不可变。
3.  **通用字符串操作与处理 (`FString`)：** 类似标准库的 `std::string` 或 `std::wstring`。用于需要动态修改、拼接、解析、序列化、与外部 API（如文件系统、网络）交互等操作的任意字符串数据。可变（Mutable），存储实际的字符序列。
4.  **字符字面量声明 (`TEXT()` 宏)：** 确保字符串字面量在 UE 的宽字符 (`TCHAR`) 环境中被正确声明，保证跨平台一致性。

---

## 1. FString

*   **本质：** 动态的、可变的、大小写敏感的 Unicode 字符串 (`TArray<TCHAR>`)。类似于 `std::wstring`。
*   **继承关系：** `FString` 是一个独立的类 (`Core\Public\Containers\UnrealString.h`)，不是从其他特定字符串基类继承而来。但它内部使用 `TArray<TCHAR>` 来存储字符数据。
*   **核心用途：**
    *   需要动态构建或修改的字符串（用户输入、文件内容、网络数据解析、动态生成路径等）。
    *   与需要 `const char*` 或 `const wchar_t*` 的第三方库或操作系统 API 交互（通过 `GetCharArray()`, `operator*()`, `TCHAR_TO_ANSI`, `TCHAR_TO_UTF8` 等转换）。
    *   序列化和反序列化（`FArchive& operator<<`）。
    *   复杂的字符串操作（查找、替换、分割、正则表达式匹配等）。
*   **关键特性：**
    *   **可变性：** 内容可以随意修改（追加、插入、删除、替换）。
    *   **大小写敏感：** 比较操作区分大小写。
    *   **存储实际字符：** 直接存储 `TCHAR` 数组。
    *   **性能：** 相对于 `FName` 和 `FText`，操作（尤其是构造、修改、比较、散列）通常较慢，因为它涉及内存管理和实际字符处理。
    *   **内存开销：** 相对较高，因为它存储完整的字符数组及其管理信息（长度、容量等）。
    *   **非本地化：** 本身不包含任何本地化元数据。如果用于 UI，需要显式转换为 `FText`（不推荐）或使用 `FText::FromString`（谨慎使用，见 `FText` 部分）。
*   **常用方法函数（部分）：**
    *   **构造/赋值：**
        *   `FString()` (空字符串)
        *   `FString(const TCHAR* In)` (从 `TCHAR` 字面量或指针构造)
        *   `FString(const FString& In)` (拷贝构造)
        *   `FString(FString&& In)` (移动构造)
        *   `FString(const FName& InName)` (从 `FName` 转换)
        *   `FString(const FText& InText)` (从 `FText` 转换 - 获取基础字符串，丢失本地化信息)
        *   `operator=`, `operator+=`
    *   **访问：**
        *   `operator[]` (获取/修改指定索引的 `TCHAR`)
        *   `GetCharArray()` (获取底层 `TArray<TCHAR>` 的 `const` 引用)
        *   `operator*()` (获取 `const TCHAR*` - 空终止字符串)
        *   `Len()` (获取字符长度)
        *   `IsEmpty()` (检查是否为空)
    *   **比较：**
        *   `operator==`, `operator!=`, `operator<`, etc. (区分大小写)
        *   `Equals(const FString& Other, ESearchCase::Type SearchCase = ESearchCase::CaseSensitive)` (指定大小写敏感度)
        *   `Compare(const FString& Other, ESearchCase::Type SearchCase = ESearchCase::CaseSensitive)` (返回整数，类似 `strcmp`)
    *   **查找：**
        *   `Find(const TCHAR* SubStr, ESearchCase::Type SearchCase = ..., ESearchDir::Type SearchDir = ..., int32 StartPosition = ...)` (查找子串)
        *   `Contains(const TCHAR* SubStr, ESearchCase::Type SearchCase = ...)` (是否包含子串)
        *   `StartsWith(const FString& InPrefix, ESearchCase::Type SearchCase = ...)` (是否以指定字符串开头)
        *   `EndsWith(const FString& InSuffix, ESearchCase::Type SearchCase = ...)` (是否以指定字符串结尾)
    *   **修改：**
        *   `Append(const FString& Str)` / `operator+=` (追加)
        *   `InsertAt(int32 Index, const FString& Str)` (在指定位置插入)
        *   `RemoveAt(int32 Index, int32 Count = 1, bool bAllowShrinking = true)` (删除字符)
        *   `Replace(const TCHAR* From, const TCHAR* To, ESearchCase::Type SearchCase = ...)` (替换子串)
        *   `ReplaceChar(TCHAR SearchChar, TCHAR ReplaceChar)` (替换字符)
        *   `ToLower()` / `ToUpper()` (转换大小写 - **原地修改**)
        *   `TrimStartAndEnd()` / `TrimTrailing()` / `TrimStart()` (去除空白字符)
        *   `TrimQuotes(bool* bQuotesRemoved = nullptr)` (去除首尾引号)
    *   **分割/组合：**
        *   `ParseIntoArray(TArray<FString>& OutArray, const TCHAR* pchDelim, bool InCullEmpty = true)` (按分隔符分割字符串到数组)
        *   `Join(const TArray<FString>& SourceArray, const FString& Separator)` (静态方法，用分隔符合并字符串数组)
    *   **转换：**
        *   `IsNumeric()` (是否可解析为数字)
        *   `ToInt(int32& OutValue)` / `ToFloat(float& OutValue)` / `ToDouble(double& OutValue)` (转换为数值)
        *   `Printf(const TCHAR* Fmt, ...)` (格式化字符串 - **类似 `sprintf`，返回新的 `FString`**)
        *   `SanitizeFloat(double InFloat, const int32 InMinFractionalDigits = ...)` (格式化浮点数)
        *   `FromInt(int32 Num)` / `FromFloat(float Num)` (静态方法，数值转字符串)
        *   `FormatAsNumber(int32 InValue)` (带千位分隔符格式化数字 - **返回 `FText`**)
    *   **内存：**
        *   `Empty(int32 Slack = 0)` (清空字符串，可选保留容量)
        *   `Shrink()` (释放多余内存)
        *   `Reserve(int32 CharacterCount)` (预分配内存)
        *   `GetAllocatedSize()` (获取分配的总内存大小)
*   **最佳实践：**
    *   用于需要动态构建或修改的字符串。
    *   用于与文件系统、网络、数据库、命令行参数等外部世界交互。
    *   用于内部数据处理、日志记录、调试输出。
    *   **避免**在需要频繁比较或作为键（如 `TMap` 键）的地方使用，优先考虑 `FName`。
    *   **避免**直接在需要显示给用户的 UI 元素中使用，优先使用 `FText` 或其转换方法（`FText::FromString` 仅用于永远不会被翻译的纯技术性字符串）。

---

## 2. FText

*   **本质：** 不可变的（Immutable）、本地化感知的文本容器。核心目标是安全地存储、格式化和向最终用户**展示**文本，支持多语言翻译。
*   **继承关系：** `FText` 是一个独立的类 (`Core\Public\Internationalization\Text.h`)，不是从其他特定字符串基类继承。其内部实现复杂，封装了本地化数据和格式化逻辑。
*   **核心用途：**
    *   所有需要显示给玩家看的文本：UI 元素（按钮文本、标签、提示）、HUD、对话、物品描述、错误消息、成就名称等。
    *   需要根据语言、地区进行翻译的文本。
    *   需要根据区域性（Culture）进行格式化的文本（日期、时间、数字、货币、百分比、复数形式）。
*   **关键特性：**
    *   **不可变性：** 一旦创建，其内容不能直接修改。所有"修改"操作（如格式化、大小写转换）都返回一个新的 `FText` 实例。这是保证线程安全和本地化一致性的关键。
    *   **本地化：** 内置对 Unreal 本地化系统（LOCTEXT 宏、文本本地化工具）的支持。包含源字符串、翻译键（Namespace/Key）、翻译文本等信息。
    *   **区域性感知：** 格式化操作（数字、日期等）会根据当前活动的区域性自动应用正确的格式规则。
    *   **安全性：** 设计上防止常见的本地化错误（如硬编码字符串、拼接已翻译文本）。
    *   **显示目的：** 主要目标是渲染显示，而不是任意的字符串处理。
    *   **性能：** 创建和格式化可能涉及查找翻译或格式化规则，通常比 `FName` 慢，但与 `FString` 的复杂操作相比可能各有优劣。比较操作通常比 `FString` 快，因为它可以比较内部表示（如键或散列）。
    *   **内存开销：** 可能包含额外的本地化元数据，但通常比存储相同内容的 `FString` 更高效（尤其对于常用文本，利用共享存储）。
*   **常用方法函数（部分）：**
    *   **构造：**
        *   `FText()` (空文本)
        *   `FText::FromString(const FString& InString)` (**谨慎使用！** 创建一个"非本地化"的 `FText`。仅适用于**永远不会被翻译**的纯技术性字符串（如代码生成的唯一ID、内部格式化的文件名）。会丢失 `InString` 的原始大小写信息，比较时可能不直观)。
        *   `FText::AsCultureInvariant(const FString& InString)` (创建一个"区域性不变"的 `FText`。用于**必须原样显示**的字符串（如玩家名、IP地址、控制台命令）。不参与本地化，按原样显示。比较区分大小写)。
        *   `LOCTEXT("Key", "SourceText")` (**最重要的宏！** 在头文件或源文件中定义本地化文本。`Key` 在 `Namespace` (通常为当前模块名) 内唯一标识该文本，`SourceText` 是源语言（通常是英语）文本。引擎的文本收集系统会抓取这些宏来生成翻译表)。
        *   `NSLOCTEXT("Namespace", "Key", "SourceText")` (显式指定命名空间的 `LOCTEXT` 版本)。
        *   `FText::GetEmpty()` (获取空文本实例)。
    *   **格式化：** (核心功能，返回新 `FText`)
        *   `FText::Format(FormatPattern, Arg1, Arg2, ...)` (**最常用**) 使用模式字符串和参数创建格式化文本。参数可以是 `FText`, `FString`, `int32`, `float`, `double` 等基本类型。模式字符串使用 `{0}`, `{1}` 等占位符。**模式字符串本身也必须是 `FText` (通常来自 `LOCTEXT`)，这样才能被翻译！** 格式化规则（如数字格式）根据当前区域性应用。
        *   `FText::FormatOrdered(FormatPattern, Arg1, Arg2, ...)` (类似 `Format`, 但严格依赖参数顺序 `{0}`, `{1}`，不依赖命名参数)。
        *   `FText::AsNumber(Number, [Options])` (格式化数字，可选项包括小数位数、是否分组等)。
        *   `FText::AsCurrency(Number, CurrencyCode, [Options])` (格式化货币)。
        *   `FText::AsDate(DateTime, [Options])` (格式化日期)。
        *   `FText::AsTime(DateTime, [Options])` (格式化时间)。
        *   `FText::AsDateTime(DateTime, [Options])` (格式化日期时间)。
        *   `FText::AsPercent(Number, [Options])` (格式化百分比)。
        *   `FText::ToLower()` / `ToUpper()` (**返回新的 `FText`**，大小写转换规则基于当前区域性)。
    *   **复数形式：** (核心功能)
        *   `FText::FormatPlural(PluralPattern, Number, [Argument])` / `FText::FormatPluralNamed(PluralPattern, Number, Name, Argument)` 处理不同语言的复数规则。`PluralPattern` 是一个包含 `|` 分隔符的 `FText`，格式通常为 `"Singular|Plural"` 或更复杂的规则定义。引擎根据当前语言和 `Number` 的值自动选择正确的形式。
    *   **比较：**
        *   `operator==`, `operator!=` (**语义等价比较**：比较的是文本的*含义*是否相同，考虑翻译和本地化上下文。通常基于内部键或散列，速度快)。
        *   `IdenticalTo(const FText& Other)` (**严格相等**：比较底层表示是否完全相同，包括是否来自相同的源/键。很少需要)。
        *   `CompareTo(const FText& Other)` / `CompareToCaseIgnored(const FText& Other)` (基于当前区域性的排序规则进行比较，返回整数。速度相对较慢)。
    *   **转换：**
        *   `ToString()` (**谨慎使用！** 将 `FText` 转换为 `FString`，返回的是**当前活动语言下的字符串表示**。**仅**在需要将最终显示的文本传递给只接受 `FString` 的 API（如某些日志、文件写入、特定渲染API）时使用。**切勿**将 `ToString()` 的结果用于逻辑比较或拼接，这会丢失所有本地化信息)。
        *   `BuildSourceString()` (获取源字符串 - 通常是 `LOCTEXT` 中的 `SourceText`)。
        *   `IsNumeric()` / `ToDouble(double& OutValue)` (尝试转换为数字 - 基于当前区域性的数字格式)。
    *   **本地化信息：**
        *   `IsCultureInvariant()` (是否是区域性不变的)。
        *   `IsTransient()` (是否是临时的)。
        *   `GetSourceString()` (同 `BuildSourceString()`)。
*   **最佳实践：**
    *   **所有面向用户的文本必须使用 `FText` 和 `LOCTEXT`/`NSLOCTEXT`。**
    *   优先使用 `Format`, `FormatPlural` 等方法来构建动态文本，确保模式字符串本身可翻译。
    *   使用 `AsCultureInvariant` 处理必须原样显示的非语言相关字符串（玩家名、代码标识符）。
    *   **避免**使用 `FromString` 创建显示文本，除非是绝对确定不需要翻译的技术字符串。
    *   **避免**在游戏逻辑中使用 `ToString()` 的结果进行比较或处理。游戏逻辑应使用 `FName` 或 `FString` 处理标识符，使用数值处理数据。
    *   理解不可变性：任何"修改"操作都返回新对象。
    *   利用复数形式处理不同语言的复数规则。

---

## 3. FName

*   **本质：** 轻量级的、不可变的、大小写不敏感的字符串**标识符**。内部存储为一个索引（指向全局字符串表）和一个实例编号（用于区分可能指向同一字符串表条目的不同 `FName`）。
*   **继承关系：** `FName` 是一个独立的类 (`Core\Public\UObject\NameTypes.h`)。
*   **核心用途：**
    *   对象名称 (`AActor::GetName()`, `UObject::GetFName()`)。
    *   属性名称 (在蓝图中暴露，反射系统)。
    *   资源引用 (纹理、材质、声音、骨骼网格体等的路径中的名称部分)。
    *   标签 (`Actor` 的 `Tags` 数组)。
    *   动画槽位名、骨骼名、物理碰撞通道名、输入动作/轴名、枚举条目名等。
    *   需要**极快**查找、比较和散列操作的场景（如 `TMap<FName, ...>`, `TSet<FName>`）。
*   **关键特性：**
    *   **不可变性：** 一旦创建，内容无法更改。
    *   **大小写不敏感：** 创建和比较操作都不区分大小写。内部存储的字符串通常是创建时传入字符串的大小写折叠（通常是小写）版本。
    *   **全局字符串表：** 所有唯一的 `FName` 字符串值存储在全局的、大小写折叠的字符串表中。`FName` 对象本身只存储一个指向该表中条目的索引和一个实例编号。这是其高效性的核心。
    *   **极高性能：**
        *   **构造：** 首次构造特定字符串的 `FName` 需要查找或添加全局表（有一定开销），后续构造相同字符串的 `FName` 非常快（直接查索引）。
        *   **比较：** `operator==` 和 `operator!=` 比较的是索引值（整数），速度极快 (`O(1)`)。`Compare` 方法比较实际字符串内容 (`O(n)`)，但应优先使用 `==`。
        *   **散列：** 散列计算基于索引值，非常快且冲突少。非常适合作为 `TMap`/`TSet` 的键。
    *   **内存高效：** 每个 `FName` 实例本身只占用少量内存（通常 8 字节：4字节索引 + 4字节实例编号）。全局字符串表有开销，但被所有 `FName` 实例共享。
    *   **非本地化：** 不包含任何翻译信息。`FName` 的值就是其标识符本身。
*   **常用方法函数（部分）：**
    *   **构造：**
        *   `FName()` (空名称 `NAME_None`)。
        *   `FName(const TCHAR* InName, EFindName FindType = FNAME_Find)` (**最常用**：`FindType` 可以是 `FNAME_Find` (仅查找，不存在则返回 `NAME_None`), `FNAME_Add` (查找并添加), `FNAME_Replace_Not_Safe_For_Threading` (查找并添加或替换旧条目 - **线程不安全，慎用！**)).
        *   `FName(const FName& InName)` (拷贝构造 - 非常快，拷贝索引)。
        *   `FName(FNameEntryId InIndex, FNameEntryId InNumber)` (低级构造)。
    *   **访问：**
        *   `operator*()` (获取 `const TCHAR*` 指向大小写折叠后的字符串内容 - **警告：内容可能不是你传入时的原始大小写！**).
        *   `ToString()` (返回 `FString`，包含大小写折叠后的字符串内容)。
        *   `ToUnstableString()` (返回 `FString`，尝试恢复原始传入的大小写，**不稳定**，不应用于比较)。
        *   `GetComparisonIndex()` / `GetDisplayIndex()` (获取内部索引 - 高级/内部使用)。
        *   `GetNumber()` (获取实例编号)。
    *   **比较：**
        *   `operator==`, `operator!=` (**索引比较**：极快，判断是否指向全局表中同一个字符串条目)。
        *   `IsEqual(const FName& Other)` (同 `operator==`)。
        *   `Compare(const FName& Other)` (比较实际字符串内容，基于大小写折叠后的字符串 - 速度慢于 `==`)。
    *   **静态方法：**
        *   `FName::NAME_None` (预定义的空名称)。
        *   `FName::NameToDisplayString(const FString& InName, bool bIsBool = false)` (将下划线分隔的 `FName` 风格字符串转换为带空格的可读字符串，如 `"HealthBar"` -> `"Health Bar"`。常用于 UI 显示非 `FText` 的标识符)。
*   **最佳实践：**
    *   用于所有需要唯一标识符、快速查找和比较的地方（对象名、属性名、标签、资源引用名、枚举名等）。
    *   优先使用 `operator==` 进行快速比较。
    *   避免使用 `ToString()` 或 `operator*()` 的结果进行逻辑比较，除非你明确需要比较字符串内容（这很慢）。使用 `FName` 的 `==` 进行标识符比较。
    *   理解其大小写不敏感性。创建后，原始传入的大小写信息通常丢失（可通过不稳定方法尝试获取，但不保证）。
    *   避免滥用：不要将 `FName` 用于需要显示给用户的长文本、需要本地化的文本、需要动态修改的文本或需要区分大小写的场景。这些是 `FText` 和 `FString` 的领域。
    *   构造时，对于预期可能不存在的名称（如从数据文件读取），优先使用 `FNAME_Find` 并检查是否为 `NAME_None`。对于确定存在的名称或需要确保存在的名称，使用 `FNAME_Add`。
    *   使用 `NameToDisplayString` 将 `FName` 风格的标识符转换为可读的 UI 字符串（仅当该标识符**不需要**本地化时）。

---

## 4. TEXT() 宏

*   **本质：** 不是一个数据类型，而是一个**预处理器宏** (`Core\Public\Templates\UnrealTemplate.h`)。用于声明宽字符 (`TCHAR`) 字符串字面量。
*   **用途：** 确保字符串字面量在 UE 的字符编码环境中被正确声明。
*   **工作原理：**
    *   在 Windows 等 `TCHAR` 定义为 `wchar_t` (通常是 UTF-16) 的平台，`TEXT("Hello")` 展开为 `L"Hello"`。
    *   在 `TCHAR` 定义为 `char` (通常是 UTF-8) 的平台 (如 Linux, macOS)，`TEXT("Hello")` 展开为 `"Hello"` (但引擎内部处理时通常仍按 UTF-8 处理)。
    *   它保证了代码在跨平台编译时，字符串字面量能正确地适应目标平台的 `TCHAR` 定义。
*   **使用场景：**
    *   在代码中声明任何传递给需要 `TCHAR*` 或 `FString`/`FName`/`FText` 构造函数的字符串字面量。
    *   示例：
        ```cpp
        FString MyStr = TEXT("This is a dynamic string");
        FName MyName = FName(TEXT("PlayerCharacter"));
        FText MyText = LOCTEXT("GreetingKey", TEXT("Hello World!")); // LOCTEXT 内部也需要 TEXT
        UE_LOG(LogTemp, Warning, TEXT("Player %s has %d health"), *PlayerName, PlayerHealth); // UE_LOG 需要 TEXT 包裹格式字符串
        ```
*   **最佳实践：**
    *   **始终**使用 `TEXT()` 宏包裹代码中的字符串字面量。
    *   这是 UE 编码规范的基本要求，确保跨平台兼容性和字符编码一致性。

---

## 总结比较与选择指南

| 特性            | FString                                      | FText                                                 | FName                                         | TEXT() 宏                    |
| :-------------- | :------------------------------------------- | :---------------------------------------------------- | :-------------------------------------------- | :--------------------------- |
| **核心目的**    | 通用、可变字符串操作与处理                   | **面向用户的文本展示与本地化** (不可变)               | **高性能标识符与比较** (不可变，大小写不敏感) | 声明宽字符 (`TCHAR`) 字面量  |
| **可变性**      | **可变** (可修改内容)                        | **不可变** (操作返回新实例)                           | **不可变**                                    | N/A (字面量)                 |
| **大小写敏感**  | **敏感**                                     | 操作可能敏感 (取决于区域性)                           | **不敏感** (创建/比较时折叠)                  | N/A                          |
| **本地化支持**  | 无                                           | **核心特性** (翻译、格式化、复数、区域性)             | 无                                            | N/A                          |
| **内部存储**    | `TArray<TCHAR>` (实际字符数组)               | 复杂 (可能包含源字符串、键、翻译、格式化信息)         | 索引 (指向全局字符串表条目) + 实例编号        | 编译器处理的字面量           |
| **性能 (比较)** | 较慢 (`O(n)`)                                | 较快 (通常基于键/索引 `O(1)` 或 `O(log n)`)           | **极快** (`O(1)` 索引比较)                    | N/A                          |
| **性能 (构造)** | 中等                                         | 中等 (可能涉及查找)                                   | 首次构造特定字符串中等，后续极快 (查索引)     | 编译时处理                   |
| **性能 (散列)** | 较慢                                         | 较快                                                  | **极快**                                      | N/A                          |
| **内存开销**    | 较高 (存储字符数组 + 管理信息)               | 中等 (可能共享存储)                                   | **极低** (实例小，共享全局表)                 | N/A                          |
| **线程安全**    | 非线程安全 (因可变性)                        | **线程安全** (因不可变性)                             | **线程安全** (因不可变性)                     | N/A                          |
| **何时使用**    | 动态构建/修改字符串；文件/网络 I/O；数据解析 | **所有 UI 文本；玩家可见信息；需要翻译/格式化的文本** | 对象/属性名；标签；资源引用；枚举；快速查找键 | **包裹所有代码字符串字面量** |

**选择流程：**

1.  **这个字符串最终要显示给玩家看吗？**
    *   **是** -> **`FText`** (使用 `LOCTEXT`/`NSLOCTEXT` 定义)。
    *   **否** -> 转到 2。
2.  **这个字符串是一个需要快速查找、比较或作为唯一标识符的“名字”吗？** (如对象名、属性名、标签、资源路径名、枚举值、输入绑定名)
    *   **是** -> **`FName`**。
    *   **否** -> 转到 3。
3.  **这个字符串需要动态创建、修改、拼接，或者用于与文件系统、网络、数据库等外部系统交互吗？**
    *   **是** -> **`FString`**。
    *   **否** -> 如果只是简单的、不变的、非标识符的技术常量字符串，`FString` 或 `const TCHAR*` (用 `TEXT` 包裹) 都可以，但通常用 `FString` 更方便管理。如果该字符串需要**原样显示**且**不翻译**（如玩家名），则用 `FText::AsCultureInvariant`。

**关键原则：**

*   **UI = `FText`**
*   **Names/Tags/IDs = `FName`**
*   **Mutable/String Ops/I/O = `FString`**
*   **Literals = `TEXT("...")`**

理解并正确应用这些字符串类型是编写高质量 UE5 代码的基础。遵循这些准则能显著提高代码的性能、可维护性和本地化支持能力。