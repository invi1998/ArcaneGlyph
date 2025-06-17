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