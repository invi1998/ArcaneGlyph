# `AddMappingContext` 和 RegisterInputMappingContext



在 Unreal Engine 5 的 Enhanced Input 系统中，`AddMappingContext` 和 `RegisterInputMappingContext` 有着**根本性的区别**，服务于不同的目的：

1.  **`UEnhancedInputLocalPlayerSubsystem::AddMappingContext`**
    
    *   **目的：** **动态地为特定本地玩家添加一个输入映射上下文到其 *活动输入栈* 中，并控制其优先级和激活状态。** 这是运行时管理玩家当前输入行为的核心方法。
    *   **所属类：** `UEnhancedInputLocalPlayerSubsystem` (继承自 `ULocalPlayerSubsystem`)。这是每个本地玩家都有的子系统实例。
    *   **作用域：** **作用于单个本地玩家**。调用它会影响调用它的那个特定玩家控制器所控制的 Pawn 的输入处理。
    *   **关键功能：**
        *   将指定的 `UInputMappingContext` 添加到该玩家的活动映射上下文列表中。
        *   通过 `Priority` 参数控制该上下文在栈中的位置（优先级高的覆盖优先级低的绑定）。
        *   通过 `ModifyContextOptions` 参数（如 `bIgnoreAllPressedKeysUntilRelease`）控制添加上下文时如何处理当前按键状态。
        *   管理上下文的激活状态（虽然通常添加即激活，但可能有内部状态管理）。
        *   直接影响该玩家*此刻*如何处理输入事件。
    *   **典型使用场景：**
        *   当玩家获得一件武器时，添加该武器的特定输入映射。
        *   进入驾驶载具时，添加驾驶相关的映射。
        *   打开菜单时，添加 UI 导航映射并暂停游戏操作映射。
        *   切换角色能力状态时更新输入映射。
    *   **参数：**
        ```cpp
        void AddMappingContext(UInputMappingContext* MappingContext, int32 Priority, const FModifyContextOptions& Options = FModifyContextOptions());
        ```
    
2.  **`IEnhancedInputSubsystemInterface::RegisterInputMappingContext`**
    *   **目的：** **向 *输入系统* 注册一个 `UInputMappingContext` 资产，使其被系统知晓并可用于运行时管理。** 它更多是关于资源管理和初始化，而不是运行时动态控制玩家输入。
    *   **所属接口：** `IEnhancedInputSubsystemInterface`。这是一个接口，**主要的实现者是 `UEnhancedInputEditorSubsystem`**。`UEnhancedInputLocalPlayerSubsystem` *不* 实现这个接口方法。
    *   **作用域：** **作用于全局输入系统或编辑器子系统**。注册一个上下文使其成为一个可用的资源。
    *   **关键功能：**
        *   通知输入系统（尤其是在编辑器环境中）存在这个 `UInputMappingContext` 资源。
        *   可以被编辑器工具或某些需要预知所有可能上下文的高级系统功能使用。
        *   **它 *不* 会将上下文添加到任何玩家的活动输入栈中。它 *不* 直接影响任何玩家当前的输入处理。**
        *   主要与 **`UEnhancedInputEditorSubsystem`** 相关，用于在编辑器内预览或模拟输入行为。
    *   **典型使用场景：**
        *   **编辑器工具或插件**：在编辑器中构建、测试或可视化输入配置时，确保所有相关的映射上下文被加载并注册到编辑器输入子系统 (`UEnhancedInputEditorSubsystem`)。
        *   **非常规的游戏运行时初始化**：可能在极少数情况下，某种自定义的全局管理器需要在游戏启动时显式注册所有上下文（但通常引擎会自动处理已引用的资产）。
    *   **参数：**
        ```cpp
        virtual void RegisterInputMappingContext(const UInputMappingContext* IMC) = 0;
        ```

**核心区别总结：**

| 特性               | `AddMappingContext`                                 | `RegisterInputMappingContext`                                |
| :----------------- | :-------------------------------------------------- | :----------------------------------------------------------- |
| **主要目的**       | **运行时动态管理玩家输入栈** (添加/移除/控制优先级) | **注册输入映射上下文资源到系统** (使其被知晓)                |
| **直接影响**       | **直接影响特定玩家 *当前* 的输入处理行为**          | **不影响任何玩家当前的输入处理**                             |
| **所属对象**       | `UEnhancedInputLocalPlayerSubsystem` (每个本地玩家) | `IEnhancedInputSubsystemInterface` (主要实现: `UEnhancedInputEditorSubsystem`) |
| **作用域**         | **单个本地玩家**                                    | **全局输入系统 / 编辑器子系统**                              |
| **典型使用位置**   | **游戏逻辑代码** (角色、控制器、游戏模式、组件等)   | **编辑器工具代码** / 特殊的全局初始化代码                    |
| **是否控制优先级** | **是**                                              | **否**                                                       |
| **是否激活绑定**   | **是** (添加后通常立即生效)                         | **否** (仅注册资源)                                          |

**简单来说：**

*   你想让**玩家**的输入**现在**做某些事情（比如按空格键跳跃）？ ➔ 用 `AddMappingContext` 把包含跳跃绑定的上下文加到那个玩家的 `EnhancedInputLocalPlayerSubsystem` 上。
*   你在构建一个**编辑器工具**，需要让编辑器知道存在某个输入映射上下文资源以便预览？ ➔ 用 `RegisterInputMappingContext` 把它注册到 `EnhancedInputEditorSubsystem`。

**在绝大多数游戏逻辑编程中，你只需要关心和使用 `UEnhancedInputLocalPlayerSubsystem::AddMappingContext()` (以及对应的 `RemoveMappingContext`)。`RegisterInputMappingContext` 主要用于编辑器内部或非常特定的工具开发场景。**