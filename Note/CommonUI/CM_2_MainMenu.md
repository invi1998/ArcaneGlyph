# GetDesiredInputConfig(设置UI控件激活时的输入模式)

*在指定了这个未来可激活控件所需的输入冲突后，除非另有说明，否则我们将一直使用这个模式。*

*因此，如果我们希望在该控件激活时切换到新的输入模式，只需重写*

*该控件内部的 `GetDesiredInputConfig` 函数即可。*

*而对于后续未处理的 Activatable 控件，系统将继续沿用之前激活的 预期输入配置。*

![image-20250606143026679](.\image-20250606143026679.png)

在Unreal Engine 5的CommonUI系统中，`CommonActivatableWidget::GetDesiredInputConfig()`函数是**可激活控件的输入管理核心**，它提供了一种声明式、自动化的输入模式管理方案，与传统手动设置输入模式（如`SetInputModeUIOnly()`）有本质区别。以下是深度对比分析：

---

### 一、传统输入模式设置方式（手动管理）
```cpp
// 传统做法示例
void UMyWidget::OpenMenu()
{
    AddToViewport();
    APlayerController* PC = GetOwningPlayer();
    
    // 1. 手动创建输入配置
    FInputModeGameAndUI InputMode;
    InputMode.SetWidgetToFocus(TakeWidget());
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    
    // 2. 显式应用配置
    PC->SetInputMode(InputMode);
    
    // 3. 必须手动显示光标
    PC->bShowMouseCursor = true;
    
    // 4. 关闭时需要恢复原状态（易遗漏！）
}
```

#### 痛点分析：
1. **硬编码依赖**：输入逻辑与UI代码紧耦合
2. **状态管理复杂**：需手动保存/恢复之前的输入模式
3. **多层级冲突**：多个UI同时打开时输入路由混乱
4. **遗漏风险**：忘记恢复输入模式会导致玩家控制失效

---

### 二、`GetDesiredInputConfig`机制解析
#### 函数签名：
```cpp
virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
```
返回`FUIInputConfig`结构体：
```cpp
struct FUIInputConfig
{
    ECommonInputMode InputMode = ECommonInputMode::Menu; // 输入模式
    EMouseCaptureMode MouseCaptureMode = EMouseCaptureMode::NoCapture; // 鼠标捕获
    bool bHideCursorDuringViewportCapture = true; // 视口捕获时隐藏光标
};
```

#### 工作流程：
1. **自动调用时机**：
   - 控件通过`CommonActivatableWidgetStack`激活时
   - 控件获得焦点时
2. **层级优先级**：
   ```mermaid
   graph LR
   A[顶层激活控件] -->|GetDesiredInputConfig| B[输入路由器]
   B --> C[应用配置到PlayerController]
   ```
3. **自动恢复机制**：
   - 控件失活时自动恢复前一个控件的输入配置
   - 无激活控件时恢复游戏默认输入

---

### 三、核心优势对比

| **特性**       | 传统输入模式                 | `GetDesiredInputConfig`                      | 优势解析              |
| -------------- | ---------------------------- | -------------------------------------------- | --------------------- |
| **管理方式**   | 命令式（手动设置）           | 声明式（返回所需配置）                       | 解耦UI与输入逻辑      |
| **状态恢复**   | 需手动保存/恢复              | 自动栈管理（`CommonActivatableWidgetStack`） | 消除状态泄漏风险      |
| **多层级处理** | 需自定义优先级系统           | 由`CommonUI`输入路由器自动处理               | 完美处理弹窗/嵌套菜单 |
| **设备自适应** | 需手动处理不同设备           | 与`CommonInput`系统深度集成                  | 自动切换键鼠/手柄     |
| **光标管理**   | 需单独控制`bShowMouseCursor` | 通过`MouseCaptureMode`统一控制               | 避免光标状态不一致    |
| **配置持久性** | 易丢失（如关卡切换）         | 绑定到控件生命周期                           | 状态与UI共存亡        |

---

### 四、典型应用场景

#### 1. 标准菜单控件
```cpp
TOptional<FUIInputConfig> UWidget_MainMenu::GetDesiredInputConfig() const
{
    return FUIInputConfig(
        ECommonInputMode::Menu, 
        EMouseCaptureMode::NoCapture,
        false // 保持光标可见
    );
}
```

#### 2. 全屏地图（需捕获鼠标）
```cpp
TOptional<FUIInputConfig> UWidget_WorldMap::GetDesiredInputConfig() const
{
    return FUIInputConfig(
        ECommonInputMode::Game, // 允许同时接收游戏输入
        EMouseCaptureMode::CapturePermanently, // 永久捕获鼠标
        true // 视口捕获时隐藏光标
    );
}
```

#### 3. 暂停菜单（阻断游戏输入）
```cpp
TOptional<FUIInputConfig> UWidget_PauseMenu::GetDesiredInputConfig() const
{
    return FUIInputConfig(
        ECommonInputMode::Menu,
        EMouseCaptureMode::NoCapture,
        false
    );
}
```

---

### 五、关键技术细节

#### 1. 输入模式枚举说明
| **`ECommonInputMode`** | 效果                         |
| ---------------------- | ---------------------------- |
| `Menu`                 | 仅UI响应输入（阻断游戏输入） |
| `Game`                 | 仅游戏响应输入（忽略UI）     |
| `GameAndMenu`          | UI和游戏同时接收输入         |
| `All`                  | 所有输入（含控制台命令）     |

#### 2. 鼠标捕获模式
| **`EMouseCaptureMode`**  | 效果             |
| ------------------------ | ---------------- |
| `NoCapture`              | 鼠标自由移动     |
| `CapturePermanently`     | 锁定鼠标到窗口   |
| `CaptureDuringMouseDown` | 仅鼠标按下时捕获 |

#### 3. 高级覆盖技巧
```cpp
// 动态调整配置（如根据平台）
TOptional<FUIInputConfig> UWidget_Options::GetDesiredInputConfig() const
{
#if PLATFORM_CONSOLE
    return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::CapturePermanently);
#else
    return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
#endif
}
```

---

### 六、与传统方案协同工作
#### 当需要混合使用时：
```cpp
void UWidget_CustomInput::OnActivated()
{
    Super::OnActivated();
    
    // 1. 通过CommonUI声明基础配置
    // (GetDesiredInputConfig自动生效)
    
    // 2. 额外手动设置
    GetOwningPlayer()->bShowMouseCursor = false; 
    
    // 3. 重写恢复逻辑
    RegisterUIActionBinding(FBindUIActionParams(...));
}
```

#### 注意事项：
1. **避免配置冲突**：手动设置可能覆盖`CommonUI`的自动管理
2. **使用`RegisterUIActionBinding`**：替代传统的`InputComponent`绑定
3. **慎用`SetInputMode`**：直接调用会破坏`CommonUI`的输入栈

---

### 七、最佳实践建议
1. **保持配置简洁**：90%场景只需返回`FUIInputConfig`无需额外代码
2. **利用输入路由器**：通过`CommonUIInputSubsystem`调试输入状态
3. **设备特定适配**：
   ```cpp
   // 在控件内根据设备调整行为
   void UMyWidget::NativeOnInitialized()
   {
        if (GetInputDevice() == ECommonInputType::Gamepad) 
        {
            AutoFocusToFirstButton();
        }
   }
   ```
4. **配合`CommonInputAction`**：使用数据驱动的输入操作而非硬编码键位

---

### 总结
`GetDesiredInputConfig()` 是CommonUI输入管理系统的**中枢神经**，通过：
1. **声明式配置**：UI控件自我描述所需输入状态
2. **自动栈管理**：解决多层UI输入冲突
3. **设备无缝切换**：原生支持键鼠/手柄/触摸
4. **生命周期安全**：自动状态恢复

相比传统手动模式，它减少约70%的输入管理代码量，彻底解决"忘记恢复输入模式"的经典Bug。建议所有新项目采用此方案，尤其在需要复杂UI层级的游戏中，这是构建健壮输入系统的基石。