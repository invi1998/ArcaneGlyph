在 Unreal Engine 5 (UE5) 中，`TSharedPtr` 不能添加 `UPROPERTY` 宏，这是由引擎的对象生命周期管理和垃圾回收 (Garbage Collection, GC) 机制决定的。以下是详细解释和相关情况：

---

### **为什么 `TSharedPtr` 不能添加 `UPROPERTY`？**

1. **所有权系统冲突**：
   - **`UPROPERTY` 的作用**：标记 UObject 指针，使其被 UE 的**自动垃圾回收系统**管理。引擎通过跟踪 `UPROPERTY` 标记的引用关系，自动销毁不再被引用的 UObject。
   - **`TSharedPtr` 的作用**：基于**引用计数**手动管理对象生命周期（通常是非 UObject 类型）。其增减引用计数时，UE 的 GC 系统无法感知。
   - **冲突原因**：若允许 `UPROPERTY TSharedPtr<UObject>`，会导致**双重所有权管理**：
     - UE 的 GC 系统尝试管理 `UObject` 的生命周期。
     - `TSharedPtr` 的引用计数也在管理同一个对象。
     两者无法协调，极易引发对象被提前释放或内存泄漏。

2. **技术限制**：
   - UE 的 GC 系统通过扫描 UProperty 内存布局追踪引用。`TSharedPtr` 是一个复杂模板类，GC 系统无法解析其内部指针（`UObject*` 被包裹在智能指针内部），导致无法正确标记对象为“可达”。

3. **设计哲学**：
   - UObject 的生命周期**必须由 UE 的 GC 系统管理**，这是引擎框架的核心规则。
   - 智能指针（`TSharedPtr`/`TWeakPtr`/`TUniquePtr`）仅适用于**非 UObject 类型**（如自定义 C++ 类、结构体等）。

---

### **其他不能添加 `UPROPERTY` 的情况**

#### 1. **非 `UObject` 指针的智能指针**
   - `TSharedPtr<FMyClass>`、`TWeakPtr<FMyClass>`、`TUniquePtr<FMyClass>`  
     **原因**：`UPROPERTY` 仅用于支持 UE 反射系统的类型（主要是 UObject 派生类）。智能指针指向的非 UObject 类型无法被 GC 管理。

#### 2. **裸指针指向非 UObject 类型**
   - `FMyClass* MyRawPointer;`  
     **原因**：`UPROPERTY` 无法管理非 UObject 对象的生命周期，添加后引擎无法处理其内存，可能导致崩溃。

#### 3. **STL 容器（`std::vector`, `std::map` 等）**
   - `std::vector<UObject*> MyVector;`  
     **原因**：
     - UE 的 GC 系统无法扫描 STL 容器的内部结构。
     - 需改用 UE 提供的容器（如 `TArray<UObject*>`），并添加 `UPROPERTY` 以保证 GC 正确追踪。

#### 4. **`TUniquePtr` 即使指向 UObject**
   - `TUniquePtr<UMyObject> MyUniquePtr;`  
     **原因**：`TUniquePtr` 是独占指针，其所有权语义与 GC 冲突，且 GC 无法追踪内部指针。

#### 5. **`TSharedPtr` 指向 UObject**
   - `TSharedPtr<UMyObject> MySharedPtr;`  
     **原因**：如前所述，双重所有权管理冲突。

#### 6. **未暴露给反射的类型**
   - 非 `UCLASS`/`USTRUCT` 的自定义类指针。  
     **原因**：`UPROPERTY` 依赖 UE 的反射系统，未通过宏注册的类型无法被 GC 识别。

---

### **正确使用模式**

#### ✅ **UObject 的正确管理方式**
```cpp
// 正确：UPROPERTY 标记裸指针，由 GC 管理
UPROPERTY()
UMyObject* MyUObjectPtr;

// 正确：对 UObject 使用弱引用（避免循环引用）
UPROPERTY()
TWeakObjectPtr<UMyObject> MyWeakUObjectPtr; // 可被 GC 感知
```

#### ✅ **非 UObject 的正确管理方式**
```cpp
// 正确：非 UObject 使用智能指针（无需 UPROPERTY）
TSharedPtr<FMyNonUObject> MySharedPtr;

// 正确：非 UObject 的弱引用
TWeakPtr<FMyNonUObject> MyWeakPtr;
```

#### ✅ **UE 容器管理 UObject 指针**
```cpp
// 正确：TArray 可被 GC 扫描
UPROPERTY()
TArray<UMyObject*> MyObjectArray;

// 正确：TSet/TMap 同样支持
UPROPERTY()
TSet<UMyObject*> MyObjectSet;
```

---

### **关键总结**
| **类型**                  | **能否加 `UPROPERTY`** | **原因**                   |
| ------------------------- | ---------------------- | -------------------------- |
| `UObject*`                | ✅ 必须                 | 由 GC 管理生命周期         |
| `TWeakObjectPtr<UObject>` | ✅ 推荐                 | GC 感知的弱引用            |
| `TSharedPtr<UObject>`     | ❌ 禁止                 | 与 GC 所有权冲突           |
| `TUniquePtr<UObject>`     | ❌ 禁止                 | 与 GC 所有权冲突           |
| `FMyClass*`（非 UObject） | ❌ 禁止                 | GC 无法管理非 UObject 内存 |
| `TSharedPtr<FMyClass>`    | ❌ 禁止                 | 非 UObject 无需 GC 管理    |
| `std::vector<UObject*>`   | ❌ 禁止                 | GC 无法扫描 STL 容器       |
| `TArray<UObject*>`        | ✅ 必须                 | GC 可追踪 UE 容器内容      |

遵循这些规则可避免内存泄漏、野指针或引擎崩溃。核心原则：**UObject 交给 GC 管理，非 UObject 用智能指针或手动管理**。



# UObject 

Unreal Engine 的核心基石：**`UObject`** 及其衍生知识。理解 `UObject` 是掌握 UE 架构的关键。

---

## **一、UObject：万物之源**

1.  **核心地位：**
    *   `UObject` 是 Unreal Engine 中几乎所有游戏相关对象的**基类**。
    *   `AActor`（场景中的对象）、`UActorComponent`（Actor的功能组件）、`UBlueprint`、`UDataAsset`、`UGameInstance`、`USkeletalMesh`、`UMaterial` 等等，都直接或间接继承自 `UObject`。
    *   它是 UE **反射系统（Reflection System）** 和 **垃圾回收（Garbage Collection, GC）** 机制的根基。

2.  **核心职责与提供的能力：**
    *   **反射 (Reflection):**
        *   在运行时获取类、属性、函数等信息。
        *   支持蓝图可视化编辑、序列化、网络复制、命令行工具、编辑器细节面板等。
        *   通过 `UCLASS()`, `UPROPERTY()`, `UFUNCTION()`, `USTRUCT()` 等宏将 C++ 代码暴露给反射系统。
    *   **垃圾回收 (Garbage Collection):**
        *   **自动内存管理：** UE 会定期扫描内存，自动销毁不再被任何 `UPROPERTY()` 引用或其他 GC Root（如游戏实例、关卡中的 Actor 列表）引用的 `UObject` 实例。
        *   **消除常见内存错误：** 大幅减少手动 `new`/`delete` 带来的野指针、内存泄漏风险。
        *   **基于引用跟踪：** GC 通过追踪 `UObject` 之间的引用关系（主要是通过 `UPROPERTY()` 标记的指针和容器）来确定对象是否“可达”。
    *   **序列化 (Serialization):**
        *   将对象状态保存到磁盘（如.uasset资源文件、.umap地图文件、存档文件）或通过网络传输。
        *   利用反射信息读写属性。
        *   支持版本向后兼容（`FArchive` 的版本控制）。
    *   **类默认对象 (Class Default Object - CDO):**
        *   每个 `UClass` 在程序启动时都会自动创建一个**单例**实例，称为 CDO。
        *   CDO 存储了该类的**默认属性值**。当你创建一个该类的 `UObject` 实例时，它的属性初始值就是从 CDO 复制过来的。
        *   蓝图编辑器中设置的默认值就存储在蓝图的 CDO 中。
        *   可以通过 `GetClass()->GetDefaultObject<UMyClass>()` 获取 CDO。
    *   **元数据 (Metadata):**
        *   通过 `UPROPERTY(meta=(...))` 或 `UCLASS(meta=(...))` 等添加的附加信息。
        *   主要用于**编辑器工具提示、分类、输入验证、UI 控件定制**等（例如 `DisplayName`, `Category`, `ToolTip`, `ClampMin`, `ClampMax`, `EditCondition`）。
    *   **网络复制 (Replication):**
        *   多玩家游戏中，服务器和客户端之间同步对象状态的基础。
        *   通过 `UPROPERTY(Replicated)` 或 `UFUNCTION(Server, Client, NetMulticast)` 等标记属性/函数，配合 `GetLifetimeReplicatedProps` 函数实现。
    *   **编辑器集成：**
        *   反射信息驱动了虚幻编辑器的细节面板、内容浏览器、蓝图图表等。
        *   允许 C++ 代码无缝与蓝图交互。

3.  **生命周期管理 (关键！):**
    *   **创建：** 必须使用 **`NewObject()`** 或 **`CreateDefaultSubobject()`** (主要在 Actor 构造函数中用于创建组件) 模板函数创建 `UObject` 实例。**禁止直接使用 `new`！**
    *   **引用：** 使用 **`UPROPERTY()`** 标记指向其他 `UObject` 的指针（裸指针或 `TWeakObjectPtr`）或包含 `UObject*` 的 UE 容器（`TArray`, `TSet`, `TMap`）。这是 GC 跟踪引用的唯一方式。
    *   **销毁：**
        *   **主要方式：** 由 GC 自动销毁（当对象不可达时）。
        *   **显式标记：** 调用 `MarkAsGarbage()` 或 `ConditionalBeginDestroy()` 可以将对象标记为“待销毁”，GC 会在下次运行时清理它。
        *   **强制立即销毁 (慎用！)：** `LowLevelDestroy()` / `Destroy()` (仅适用于 `AActor`)。这会绕过 GC 的常规流程，可能导致问题，仅在极少数特殊情况下使用（如关卡卸载）。

4.  **设计哲学：**
    *   **约定优于配置：** 通过宏和继承 `UObject`，自动获得强大功能（GC、反射、序列化）。
    *   **所有权明确：** GC 系统清晰地定义了对象的生命周期由引擎管理。开发者通过 `UPROPERTY` 声明引用关系来“影响” GC 决策。
    *   **安全：** 自动内存管理减少了 C++ 底层内存错误。

---

## **二、重要衍生概念与类型**

1.  **`AActor`：**
    *   继承自 `UObject`。
    *   代表**关卡中可放置的实体**。是游戏世界中的“物体”（角色、灯光、触发器、摄像机、静态网格体等）。
    *   核心特性：
        *   **Transform (位置、旋转、缩放)：** 存在于游戏世界中。
        *   **层级 (Attachment)：** 可以附加到其他 Actor 上，形成父子关系。
        *   **拥有 `UActorComponent`：** 通过组件 (`UActorComponent` 或其子类如 `USceneComponent`, `UPrimitiveComponent`) 添加具体功能（渲染、物理、逻辑）。
        *   **网络角色 (`ROLE_Authority`, `ROLE_SimulatedProxy`, `ROLE_AutonomousProxy`)：** 定义其在网络游戏中的权限和同步行为。
        *   **生命周期事件：** `BeginPlay()`, `Tick()`, `EndPlay()`。
        *   **由 `ULevel` 管理：** 存在于特定关卡中，随关卡加载/卸载。

2.  **`UActorComponent`：**
    *   继承自 `UObject`。
    *   代表附加到 `AActor` 上的**功能模块**。Actor 本身更像一个空壳，其具体行为由组件实现。
    *   类型：
        *   **`USceneComponent`：** 具有变换信息，构成 Actor 变换层级的基础（如 `RootComponent`）。
        *   **`UPrimitiveComponent`：** 继承自 `USceneComponent`，具有渲染或物理碰撞表示（如 `UStaticMeshComponent`, `USkeletalMeshComponent`, `UBoxComponent`）。
        *   **逻辑组件 (如 `UMyHealthComponent`)：** 通常直接继承自 `UActorComponent` 或 `UActorComponent` 的其他子类，处理特定游戏逻辑。
    *   生命周期事件： `InitializeComponent()`, `BeginPlay()`, `TickComponent()`, `EndPlay()` (通常与所属 Actor 同步)。
    *   通过 `AActor::GetComponentByClass()` 或 `TArray` 查找组件。

3.  **`UStruct`：**
    *   使用 `USTRUCT()` 宏定义。
    *   类似于 C++ 结构体，但**也支持 UE 的反射系统**（属性可被蓝图访问、可序列化、可网络复制）。
    *   **不继承自 `UObject`！** 因此：
        *   **没有 GC 管理：** `UStruct` 变量的内存由其**所有者**（通常是一个 `UObject` 或另一个 `UStruct`）管理。当所有者被销毁或离开作用域时，`UStruct` 变量也随之销毁。
        *   **没有虚函数表 (vtable)：** 通常比 `UObject` 更轻量。
        *   **没有 RTTI (Run-Time Type Information)：** 反射信息是其主要类型识别方式。
    *   常用于定义数据容器（如配置、参数、网络复制数据包）。例如 `FHitResult`, `FVector`, `FRotator` 都是 `USTRUCT`。

4.  **`TWeakObjectPtr`：**
    *   模板类 `TWeakObjectPtr`。
    *   专门用于安全地持有对 `UObject`（或其子类）的**非拥有、弱引用**。
    *   **核心价值：**
        *   **解决循环引用：** 两个 `UObject` 互相用 `UPROPERTY` 强引用对方会导致 GC 无法回收它们（内存泄漏）。使用 `TWeakObjectPtr` 打破强引用环。
        *   **安全访问：** 通过 `Get()` 方法获取裸指针。如果目标对象已被 GC 销毁，`Get()` 会返回 `nullptr`。使用前**必须检查有效性** (`IsValid()` 或 `Get() != nullptr`)。
    *   **可以添加 `UPROPERTY`：** 因为 `TWeakObjectPtr` 内部机制让 GC 能感知到它引用的对象，但知道这个引用是“弱”的，不影响目标对象的生命周期判断。GC 在目标对象销毁时会自动置空 `TWeakObjectPtr`。

5.  **`UInterface`：**
    *   使用 `UINTERFACE()` 宏定义接口，用 `class IMyInterface` 定义实际的 C++ 接口类。
    *   允许不同的、不相关的 `UObject` 类实现**相同的一组功能（函数）**。
    *   在 C++ 中通过 `Cast` (`Cast`) 或 `Implements` (`ImplementsInterface`) 检查对象是否实现了接口，并调用接口函数。
    *   在蓝图中可以通过“转换为接口”节点调用接口函数。
    *   是实现**多态**和**解耦**代码的重要手段（“面向接口编程”）。

6.  **`UClass`：**
    *   继承自 `UObject`。
    *   是 `UObject` 反射系统的核心。每个 `UCLASS()` 宏标记的类在运行时都有一个对应的 `UClass` 实例。
    *   `UClass` 对象包含了关于它所代表的类的所有反射信息：
        *   父类 (`SuperStruct`)
        *   类名 (`GetName()`)
        *   属性列表 (`TFieldIterator`)
        *   函数列表 (`FindFunctionByName()`)
        *   接口列表
        *   类标志 (`ClassFlags`)
        *   类默认对象 (CDO)
    *   可以通过 `StaticClass()` (C++) 或 `::StaticClass()` (蓝图) 获取一个类的 `UClass*`。运行时创建对象 (`NewObject`) 和类型检查 (`Cast`, `IsA`) 都依赖 `UClass`。

7.  **`UWorld`：**
    *   继承自 `UObject`。
    *   代表一个**游戏世界或关卡**的容器和上下文。
    *   管理：
        *   当前加载的 `ULevel`（关卡）。
        *   该世界中的 `AActor` 列表。
        *   物理场景 (`FPhysScene`)。
        *   导航系统 (`UNavigationSystem`)。
        *   游戏模式 (`AGameModeBase`)。
        *   玩家控制器 (`APlayerController`)。
    *   提供重要的全局函数：`SpawnActor()`, `GetFirstPlayerController()`, `GetGameInstance()`, `GetTimerManager()` 等。
    *   在编辑器中，每个打开的关卡（地图）对应一个 `UWorld`。在游戏中，主关卡和流式加载的关卡共享或拥有自己的 `UWorld` 上下文。

---

## **三、UObject 使用要点与最佳实践**

1.  **永远使用 `NewObject`/`CreateDefaultSubobject`：** 这是生命周期的起点。
2.  **正确使用 `UPROPERTY`：**
    *   所有指向其他 `UObject` 的持久引用（需要跨函数调用或帧保持的）**必须**用 `UPROPERTY()` 标记，否则 GC 无法追踪，对象会被提前销毁。
    *   对于非持久引用（如局部变量、函数参数），不需要 `UPROPERTY`，但需确保其生命周期内目标对象有效。
    *   使用 `TWeakObjectPtr` 打破循环引用或持有非拥有引用。
3.  **避免在构造函数中进行复杂操作：**
    *   此时对象及其依赖项（如其他组件、World）可能未完全初始化。
    *   初始化逻辑通常放在 `BeginPlay()` 或 `PostInitializeComponents()` 中。
4.  **理解 `BeginDestroy()` 和 `FinishDestroy()`：**
    *   GC 销毁对象前会调用这些函数进行资源清理（如释放非GC管理的资源、移除委托绑定）。可以重写它们，但要记得调用父类实现 (`Super::BeginDestroy()`)。
5.  **谨慎使用 `ForceGarbageCollection`：** 让 GC 立即运行通常用于调试内存问题，在正式游戏中会卡顿，应依赖 GC 的自动调度。
6.  **异步加载 (`FStreamableManager`, `AsyncLoad`)：** 加载大型资源（如地图、纹理、网格体）应使用异步方式，避免主线程卡顿。加载结果通常是 `UObject`（如 `UWorld`, `UTexture`）。
7.  **蓝图交互：**
    *   `UPROPERTY(BlueprintReadOnly/ReadWrite)` 暴露属性。
    *   `UFUNCTION(BlueprintCallable/BlueprintImplementableEvent/BlueprintPure)` 暴露函数/事件。
    *   `BlueprintType` 标记 `USTRUCT` 可在蓝图中作为变量类型。

---

## **四、常见问题 (FAQ)**

*   **Q：为什么我的 UObject 指针有时会变成 `nullptr`？**
    *   **A：** 最常见原因：
        1.  对象已被 GC 销毁（检查是否所有必要引用都有 `UPROPERTY` 标记？是否存在循环引用需要用 `TWeakObjectPtr` 打破？）。
        2.  对象所属的关卡 (`ULevel`) / 世界 (`UWorld`) 已被卸载。
        3.  指针是 `TWeakObjectPtr` 且目标对象已被销毁（`Get()` 前未检查 `IsValid()`）。
*   **Q：什么时候应该用 `UStruct` 而不是 `UObject`？**
    *   **A：** 当需要：
        *   一个轻量级的纯数据容器（没有复杂行为，不需要 GC 管理，不需要独立生命周期）。
        *   值语义（拷贝是独立的副本）。
        *   作为另一个 `UObject` 或 `UStruct` 的属性（嵌套数据）。
*   **Q：`CreateDefaultSubobject` 只能在构造函数里用吗？**
    *   **A：** 是的！它专门用于在 `AActor` 或 `UActorComponent` 的**构造函数**中创建并添加子组件。在运行时创建组件应使用 `NewObject` 和 `AddInstanceComponent` (并手动注册 `RegisterComponent`）。
*   **Q：`IsValid()` 和 `!= nullptr` 检查 `UObject` 指针有什么区别？**
    *   **A：** `!= nullptr` 只检查指针本身是否为 `NULL`。
    *   `IsValid()` 是 UE 提供的安全函数，它不仅检查指针非 `NULL`，**还检查该 `UObject` 是否已被标记为待销毁或已被 GC 销毁**（即 `IsPendingKill()` 状态）。**强烈建议总是使用 `IsValid()` 来检查 `UObject` 指针的有效性！** `if (MyActor && MyActor->IsValid())` 是冗余的，只需 `if (IsValid(MyActor))`。

---

## **总结表：UObject 核心特性概览**

| 特性                  | 描述                                   | 关键点                                                       |
| :-------------------- | :------------------------------------- | :----------------------------------------------------------- |
| **反射 (Reflection)** | 运行时获取类/属性/函数信息             | `UCLASS()`, `UPROPERTY()`, `UFUNCTION()`, `USTRUCT()` 宏启用；驱动编辑器、序列化、网络、命令 |
| **垃圾回收 (GC)**     | 自动管理内存，销毁不可达对象           | **必须用 `NewObject` 创建**；**持久引用需 `UPROPERTY` 标记**；避免循环引用 (用 `TWeakObjectPtr`) |
| **序列化**            | 保存/加载对象状态                      | 用于资源 (.uasset)、地图 (.umap)、存档；依赖反射             |
| **类默认对象 (CDO)**  | 每个 UClass 的单例，存储类属性的默认值 | `GetClass()->GetDefaultObject()`；新对象属性从 CDO 初始化    |
| **元数据 (Metadata)** | 附加到属性/类上的编辑器信息            | `UPROPERTY(meta=(DisplayName="Nice Name", Category="Settings"))` |
| **网络复制**          | 服务器与客户端间同步对象状态           | `UPROPERTY(Replicated)`, `UFUNCTION(Server/Client/NetMulticast)`, `GetLifetimeReplicatedProps` |
| **编辑器集成**        | 与虚幻编辑器深度交互                   | 细节面板、内容浏览器、蓝图系统都依赖反射信息                 |
| **生命周期**          | 由引擎管理                             | `NewObject` 创建 -> GC 或显式标记销毁 (`MarkAsGarbage`)；**禁用 `new`/`delete`** |
| **`TWeakObjectPtr`**  | 对 UObject 的安全弱引用                | 打破循环引用；**访问前必须 `IsValid()`/`Get() != nullptr` 检查**；**可加 `UPROPERTY`** |

理解并熟练运用 `UObject` 及其生态系统，是高效、稳定地开发 Unreal Engine 项目的核心能力。它定义了 UE 中对象的行为模式、交互方式和生命周期规则。