# AI 避障算法（AI Avoidance）

在Unreal Engine 5（UE5）中，AI 避障算法是用于实现智能体（如NPC或角色）在复杂环境中移动时避免与其他物体或角色发生碰撞的功能。UE5 提供了两种主要的避障方法：**RVO Avoidance** 和 **Detour Crowd Avoidance**。这两种方法都基于不同的技术原理，适用于不同的场景需求。以下是对它们的详细解析：

---

## 1. **RVO Avoidance（互惠速度障碍避障）**

### 概述
RVO（Reciprocal Velocity Obstacles，互惠速度障碍）是一种基于速度空间的避障算法，最初由 G. van den Berg 等人在 2008 年提出。它通过计算每个智能体的速度障碍区域来预测可能的碰撞，并调整速度以避免冲突。

在 UE5 中，RVO Avoidance 是一种轻量级的避障解决方案，适合处理少量智能体之间的动态避障问题。

### 核心原理
- **速度障碍区域**：RVO 将每个智能体视为一个圆形或椭圆形的对象，并根据其当前位置、速度和半径计算出一个“速度障碍区域”。这个区域表示如果智能体以某个速度移动，可能会与其他智能体发生碰撞。
- **互惠性**：RVO 的核心思想是“互惠性”，即每个智能体在调整自身速度时会考虑其他智能体的行为，从而避免双方同时向同一个方向调整导致新的冲突。
- **优化目标**：RVO 的目标是找到一个最优的速度，使得智能体既能避开障碍物，又能尽量接近其目标位置。

### 实现细节
- **RVOAvoidanceComponent**：在 UE5 中，`URVOAvoidanceComponent` 是实现 RVO 避障的核心组件。开发者可以将其附加到需要避障的 AI 角色上。
- **参数配置**：
  - `AvoidanceWeight`：控制避障行为的权重，值越高，智能体越倾向于避免碰撞。
  - `Radius` 和 `Height`：定义智能体的物理尺寸。
  - `Priority`：设置智能体的优先级，优先级高的智能体在避障时会更少地调整自己的路径。
- **性能特点**：
  - RVO 避障适合小规模的动态避障场景（例如几十个智能体）。
  - 它的计算复杂度较低，但在大规模场景中可能会因为智能体数量增加而变得不稳定。

### 使用场景
- 小型团队或群体的动态避障。
- 场景中有较少的动态障碍物。
- 对性能要求较高，但不需要复杂的群体行为模拟。

---

## 2. **Detour Crowd Avoidance（绕道人群避障）**

### 概述
Detour Crowd Avoidance 是基于 Recast Navigation 库中的 **Crowd Manager** 实现的一种避障算法。它主要用于处理大规模群体的动态避障问题，能够高效地管理大量智能体的路径规划和避障行为。

### 核心原理
- **导航网格（NavMesh）**：Detour Crowd Avoidance 依赖于 UE5 的导航系统，使用 NavMesh 来表示可行走区域。智能体的路径规划和避障行为都基于 NavMesh 进行。
- **局部避障**：Detour Crowd Avoidance 结合了全局路径规划和局部避障。全局路径规划由 NavMesh 提供，而局部避障则通过调整智能体的速度和方向来实现。
- **分层管理**：智能体被分为多个“人群”（Crowd），每个群体可以独立管理，从而提高性能。

### 实现细节
- **CrowdManager**：在 UE5 中，`UCrowdFollowingComponent` 是实现 Detour Crowd Avoidance 的核心组件。它与 NavMesh 系统紧密集成，允许智能体在导航网格上进行高效的路径规划和避障。
- **参数配置**：
  - `MaxAgents`：指定每个群体中最多可以容纳的智能体数量。
  - `AgentRadius` 和 `AgentHeight`：定义智能体的物理尺寸。
  - `MaxSpeed`：限制智能体的最大移动速度。
  - `CollisionQueryRange`：设置智能体检测周围障碍物的范围。
- **性能特点**：
  - Detour Crowd Avoidance 适合处理大规模群体（例如数百个智能体）。
  - 它的计算效率较高，但对内存的需求较大，尤其是在复杂场景中。

### 使用场景
- 大规模群体的动态避障（例如战场模拟、人群疏散等）。
- 场景中有复杂的静态和动态障碍物。
- 需要结合全局路径规划和局部避障的场景。

---

## 3. **RVO Avoidance vs. Detour Crowd Avoidance**

| 特性           | RVO Avoidance          | Detour Crowd Avoidance |
| -------------- | ---------------------- | ---------------------- |
| **适用场景**   | 小规模动态避障         | 大规模群体避障         |
| **依赖系统**   | 不依赖 NavMesh         | 依赖 NavMesh           |
| **计算复杂度** | 较低                   | 较高                   |
| **性能表现**   | 适合少量智能体         | 适合大量智能体         |
| **灵活性**     | 更灵活，适用于简单场景 | 更稳定，适用于复杂场景 |
| **实现难度**   | 较简单                 | 较复杂                 |

---

## 4. **选择合适的避障算法**
在实际开发中，选择哪种避障算法取决于具体的需求：
- 如果你的项目中有少量智能体需要动态避障，且场景较为简单，可以选择 **RVO Avoidance**。
- 如果你需要处理大规模群体的动态避障，或者场景中有复杂的静态和动态障碍物，建议使用 **Detour Crowd Avoidance**。

---

## 5. **总结**
UE5 提供了强大的 AI 避障工具，RVO Avoidance 和 Detour Crowd Avoidance 各有优劣，适用于不同的场景。开发者可以根据项目的具体需求选择合适的算法，并通过调整参数优化避障效果。如果你正在开发一个包含复杂群体行为的项目，建议深入研究 Detour Crowd Avoidance 的实现细节，因为它能够更好地满足大规模场景的需求。



# AIController

这是AIController中默认的寻路组件

```c++
private:

	/** Component used for moving along a path. */
	UPROPERTY(VisibleDefaultsOnly, Category = AI)
	TObjectPtr<UPathFollowingComponent> PathFollowingComponent;

```



当前项目使用Detour Crowd Avoidance避障算法，为了使用该算法，我们需要创建AIController，然后修改AI控制器的默认寻路组件为CrowdFollowingComponent。同时在构建CS里添加AI模块`AIModule`

```c++
// 该Super构造函数中的SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent")是用于设置默认的寻路组件
// 因为默认的寻路组件是PathFollowingComponent，而我们需要的是CrowdFollowingComponent，所以需要在构造函数中设置默认的寻路组件
AArcaneAIController::AArcaneAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
}
```



这段代码是Unreal Engine 5（UE5）中**自定义AI控制器**的关键实现，用于启用**Detour Crowd避障功能**。以下是逐层解析：

---

### **代码功能**
1. **核心目标**：将AI控制器的默认路径跟随组件（`PathFollowingComponent`）替换为支持人群避障的`UCrowdFollowingComponent`。
2. **实现方式**：在AI控制器的构造函数中，通过`FObjectInitializer`强制指定子对象类型。
3. **结果**：所有由该控制器控制的AI角色将使用**动态群体避障逻辑**（Detour Crowd），而非基础的路径跟随逻辑。

---

### **代码逐行解析**
```cpp
AArcaneAIController::AArcaneAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
}
```

#### **1. 构造函数定义**
- `AArcaneAIController`：自定义的AI控制器类，继承自`AAIController`或其子类。
- `const FObjectInitializer& ObjectInitializer`：UE的对象初始化工具，用于配置组件和属性。

#### **2. Super调用**
- `Super(...)`：调用父类（`AAIController`）的构造函数。
- **关键操作**：通过`SetDefaultSubobjectClass`方法，将名为`"PathFollowingComponent"`的子对象（即路径跟随组件）的默认类型从`UPathFollowingComponent`改为`UCrowdFollowingComponent`。

#### **3. 参数传递**
- `SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent")`：  
  - **泛型参数**：`<UCrowdFollowingComponent>` 指定新组件类型。
  - **字符串参数**：`"PathFollowingComponent"` 是UE内部预定义的子对象名称，代表路径跟随组件。

---

### **技术背景**
#### **为什么需要替换组件？**
- UE默认的`UPathFollowingComponent`仅支持基础路径跟随，**缺乏避障优化**。
- `UCrowdFollowingComponent`是Detour Crowd系统的核心组件，提供：
  - **动态避障**：基于导航网格（NavMesh）的实时路径修正。
  - **群体行为**：协调多个AI的移动，避免碰撞和卡顿。
  - **参数配置**：可通过`UCrowdManager`调整代理半径、速度等避障参数。

#### **FObjectInitializer的作用**
- UE在创建对象（如`AArcaneAIController`）时，用`FObjectInitializer`初始化子对象（Subobjects）。
- `SetDefaultSubobjectClass`会**覆盖父类的默认子对象类型**，但保留其名称和初始化顺序。

---

### **实际应用场景**
假设你的游戏需要实现以下功能：
1. **NPC群体移动**：大量NPC在城镇中行走时自动避开彼此。
2. **动态障碍响应**：AI绕过突然出现的障碍物（如玩家放置的物体）。
3. **复杂地形导航**：在狭窄通道或复杂结构中保持路径平滑。

**启用`UCrowdFollowingComponent`后**：
- 在行为树中调用`MoveTo`节点时，AI会自动使用Detour Crowd避障。
- 可通过`UCrowdFollowingComponent::SetAvoidanceGroup`设置避障分组（如区分敌对和友方单位）。

---

### **潜在问题与解决方案**
| **问题**           | **解决方案**                                          |
| ------------------ | ----------------------------------------------------- |
| AI未触发避障       | 检查导航网格是否烘焙，确保`CrowdManager`已激活        |
| 避障时路径抖动     | 调整`CrowdFollowingComponent`的`SeparationWeight`参数 |
| 大规模人群性能下降 | 启用LOD（细节层次），降低远处AI的避障计算频率         |

---

### **扩展建议**
若需进一步优化：
1. **自定义避障参数**：通过`UCrowdFollowingComponent`的`SetAvoidanceParameters`调整代理半径和优先级。
2. **结合RVO**：在移动组件（如`CharacterMovementComponent`）中混合使用RVO，实现更自然的避障。
3. **调试工具**：在控制台输入`AI.Crowd.DebugDraw 1`可视化避障路径和碰撞范围。

这段代码是UE5中实现高级AI避障的基石，理解它能帮助你更好地控制群体行为的底层逻辑。

![image-20250223134505604](.\image-20250223134505604.png)

# AI感知

```c++
#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "ArcaneAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class ARCANEGLYPH_API AArcaneAIController : public AAIController
{
	GENERATED_BODY()

public:
	AArcaneAIController(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> EnemyAIPerceptionComponent;	// 敌人 AI 感知组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;					// AI 感知配置：视觉

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

};
```

Cpp

```c++
// 该Super构造函数中的SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent")是用于设置默认的寻路组件
// 因为默认的寻路组件是PathFollowingComponent，而我们需要的是CrowdFollowingComponent，所以需要在构造函数中设置默认的寻路组件
AArcaneAIController::AArcaneAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	if (UCrowdFollowingComponent* CrowdFollowingComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
	}

	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;		// 是否检测敌人：开启
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;		// 检测中立单位：关闭
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;		// 检测友军：关闭
	AISenseConfig_Sight->SightRadius = 5000.0f;		// 视野半径
	AISenseConfig_Sight->LoseSightRadius = 0.0f;		// 失去视野半径，设置为0表示永远不会失去视野
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.0f;		// 外围视野角度, 360度表示全方位视野
	

	EnemyAIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyAIPerceptionComponent"));
	EnemyAIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);	// 配置感知
	EnemyAIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());	// 设置主要感知，这里设置为视觉感知
	// 当感知更新时（检测到敌人），就会调用 OnEnemyPerceptionUpdated 这个回调函数
	EnemyAIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AArcaneAIController::OnEnemyPerceptionUpdated);	// 添加感知更新事件
}

void AArcaneAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
}

```

![image-20250223184357453](.\image-20250223184357453.png)



# 通用团队ID（GenericTeam ID) 获取团队态度，用于判断敌人和友军

为了使感知点正常工作，我需要使用通用名称ID来定义哪个骨骼是敌人，哪个骨骼是友方。

在AIController的继承接口`IGenericTeamAgentInterface`源码里，`virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const`被用来获取团队对我们AI的态度，用以定义哪个团队ID，它是友方还是敌方单位。

```c++
UENUM(BlueprintType)
namespace ETeamAttitude
{
	enum Type : int
	{
		Friendly,
		Neutral,
		Hostile,
	};
}


class IGenericTeamAgentInterface
{
	GENERATED_IINTERFACE_BODY()

	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) {}
	
	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const { return FGenericTeamId::NoTeam; }

	/** Retrieved owner attitude toward given Other object */
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const
	{ 
		const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
		return OtherTeamAgent ? FGenericTeamId::GetAttitude(GetGenericTeamId(), OtherTeamAgent->GetGenericTeamId())
			: ETeamAttitude::Neutral;
	}
};
```



## Enemy团队ID设置：

所以，我们需要在子类AI控制类中重写该虚函数

```c++
	// IGenericTeamAgentInterface Begin
	// 获取团队态度，用于判断敌人和友军
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	// ~IGenericTeamAgentInterface End
```

Cpp

```c++
ETeamAttitude::Type AArcaneAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	// 如果 Other 是一个 Pawn
	if (const APawn* PawnToCheck = Cast<APawn>(&Other))
	{
		// 如果 PawnToCheck 有一个 IGenericTeamAgentInterface 接口
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(PawnToCheck->GetController()))
		{
			// return Super::GetTeamAttitudeTowards(*PawnToCheck->GetController());

			// 如果 PawnToCheck 的团队 ID 不等于当前控制器的团队 ID
			if (TeamAgent->GetGenericTeamId() != GetGenericTeamId())
			{
				return ETeamAttitude::Hostile;
			}
		}
	}

	return ETeamAttitude::Friendly;
}
```

然后设置好团队ID获取策略后，在构造函数里设置好当前AI控制器的团队ID，比如：目前我们在做的是Enemy的AI控制器，那么我把这个Enemy的团队ID设置为1，如下：

```c++
AIController::SetGenericTeamId(FGenericTeamId(1));	// 设置团队 ID: 1, 用于区分敌人和友军,团队 ID 为 0 表示友军，团队 ID 为 1 表示敌人
```



## 玩家控制器团队ID设置

然后，我们需要给我们的玩家控制器也添加这个团队ID，所以需要玩家控制器也实现`IGenericTeamAgentInterface`接口

```c++
#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "ArcaneHeroController.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API AArcaneHeroController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AArcaneHeroController();

	// IGenericTeamAgentInterface Begin
	// 获取团队 ID
	virtual FGenericTeamId GetGenericTeamId() const override;
	// ~IGenericTeamAgentInterface End

protected:
	virtual void BeginPlay() override;

private:
	FGenericTeamId HeroTeamID;	// 英雄团队 ID（玩家团队 ID）
	
};
```



```c++
AArcaneHeroController::AArcaneHeroController()
{
	// IGenericTeamAgentInterface::SetGenericTeamId(0);
	// 这里和AIController中的设置团队 ID 的方式不同，AIController中是通过 AAIController::SetGenericTeamId 来设置团队 ID
	// 而因为我们这里没有继承自 AAIController，而 IGenericTeamAgentInterface::SetGenericTeamId 设置团队 ID 是一个空函数，所以这里设置团队 ID 是无效的
	// 所以我们在玩家控制器里新增一个成员变量来保存团队 ID FGenericTeamId HeroTeamID;	// 英雄团队 ID（玩家团队 ID）
	HeroTeamID = FGenericTeamId(0);
}

FGenericTeamId AArcaneHeroController::GetGenericTeamId() const
{
	return HeroTeamID;
}
```

启动游戏，按下`'`撇号键，进入Debug面板，然后按数字键`5`，打开AI感知调试面板，就可以看到当前AIController对我们的感知态势了。（顺便一提：按下1：打开或者关闭AI调试，按下2，打开或者关闭行为树调试，按下3：打开或者关闭Abilities，按下4：打开或者关闭EQS（环境查询））

![image-20250223193214859](.\image-20250223193214859.png)

如图，敌人正在感知我们的玩家和远方的DarkNight，它身边的骷髅兵单位为友军单位，没有在它的感知策略里，因为当前我只给骷髅兵添加了AIController，远方的DarkNight并没有添加AI，所以，在骷髅兵的视角，我和那个DarkNight都是敌方单位，这也正符合我们代码里关于获取团队ID的逻辑。



# 行为树（BehaviorTree）和黑板（BlackBoard）

好的！在Unreal Engine 5（UE5）中，**Behavior Tree（行为树）**和**Blackboard（黑板）**是构建复杂AI逻辑的核心工具。它们共同协作，实现AI的决策制定和数据管理。以下是它们的详细解析：

---

### **1. Behavior Tree（行为树）**
#### **基本概念**
行为树是一种**树状逻辑结构**，用于定义AI的决策流程。它通过**节点**（Nodes）的组合实现条件判断、任务执行和流程控制，类似于流程图。

#### **关键节点类型**
| 节点类型      | 功能描述                                                     |
| ------------- | ------------------------------------------------------------ |
| **Root**      | 行为树的起点，所有逻辑从Root开始执行                         |
| **Selector**  | 按顺序执行子节点，直到某个子节点成功（类似逻辑“或”）         |
| **Sequence**  | 按顺序执行子节点，直到某个子节点失败（类似逻辑“与”）         |
| **Task**      | 具体行为（如移动、攻击、等待）                               |
| **Decorator** | 附加在节点上的条件判断（如“是否看到玩家？”），决定是否允许执行子节点 |
| **Service**   | 后台持续运行的任务（如定期更新目标位置），不影响主流程但提供数据支持 |

#### **典型应用场景**
- **敌人AI**：巡逻 → 发现玩家 → 追击 → 攻击 → 返回巡逻
- **NPC交互**：对话 → 移动至目标 → 播放动画 → 结束任务
- **复杂决策**：根据血量选择逃跑或战斗，优先攻击最近目标

#### **UE5中的实现**
1. **创建行为树**：右键点击内容浏览器 → **AI** → **Behavior Tree**
2. **节点连接**：拖拽节点并连线，设置参数（如移动目标的Blackboard键）
3. **绑定到AI控制器**：在AI控制器的`RunBehaviorTree`函数中指定行为树资源

---

### **2. Blackboard（黑板）**
#### **基本概念**
黑板是一个**键值对存储系统**，用于在行为树节点之间共享数据（如目标位置、敌人引用、状态标志）。它与行为树解耦，可被多个AI复用。

#### **核心功能**
- **数据类型**：支持`Vector`、`Object`、`Bool`、`Float`等基础类型
- **动态更新**：通过Task节点或外部代码（如蓝图/C++）修改值
- **作用域**：每个AI实例拥有独立的Blackboard副本

#### **常见键值示例**
| 键名             | 类型   | 用途               |
| ---------------- | ------ | ------------------ |
| `TargetActor`    | Object | 当前追击的玩家对象 |
| `MoveToLocation` | Vector | 移动目标的坐标     |
| `IsAlerted`      | Bool   | 是否处于警戒状态   |
| `CurrentAmmo`    | Int    | 剩余弹药数量       |

#### **UE5中的实现**
1. **创建黑板**：右键点击内容浏览器 → **AI** → **Blackboard**
2. **定义键值**：点击`New Key`，设置名称和类型
3. **绑定到行为树**：在行为树的`Blackboard Asset`属性中选择黑板资源

---

### **Behavior Tree与Blackboard协作流程**
```cpp
// 示例：AI追击玩家的逻辑链
1. [Decorator] "HasTarget?" → 检查Blackboard的`TargetActor`是否为有效对象
   ↓ 如果为真
2. [Sequence]
   ├─ [Task] "MoveTo Target" → 使用`TargetActor`位置更新移动
   ├─ [Service] "Check Distance" → 每0.5秒更新Blackboard的`DistanceToTarget`
   └─ [Task] "Shoot" → 当距离小于5米时触发攻击
```

---

### **高级技巧与优化**
#### **1. 分层行为树**
- 使用`Subtree`节点复用通用逻辑（如“战斗模式”、“逃跑模式”）
- 通过`Behavior Tree Component`动态切换子树

#### **2. 异步任务**
- 使用`BTTask_BlueprintBase`创建非阻塞任务（如播放动画时并行检测伤害）

#### **3. 调试工具**
- 在游戏运行时按 **“'”**（反引号键）打开控制台，输入：
  - `AI.Debug.BT.ShowDebug 1` → 显示行为树当前执行路径
  - `AI.Debug.Blackboard 1` → 显示所有Blackboard键值

#### **4. 性能优化**
- 避免高频更新键值（如用`Service`的`Interval`参数限制检测频率）
- 对非必要AI禁用行为树（通过`StopBehaviorTree`）

---

### **对比传统状态机**
| **特性**    | **行为树**                          | **状态机**                     |
| ----------- | ----------------------------------- | ------------------------------ |
| **复杂度**  | 适合中大型逻辑（模块化扩展）        | 适合简单逻辑（易陷入状态爆炸） |
| **可读性**  | 树状结构直观，支持注释              | 状态转移图可能混乱             |
| **调试**    | 实时可视化执行路径                  | 需手动打印状态日志             |
| **UE5集成** | 原生支持Decorator/Service等高级特性 | 需自定义实现                   |

---

### **实际案例：潜行敌人AI**
1. **Blackboard键值**：
   - `CanSeePlayer`（Bool）
   - `LastKnownPosition`（Vector）
2. **行为树逻辑**：
   ```
   [Selector]
   ├─ [Sequence] (追击)
   │  ├─ [Decorator] "CanSeePlayer == True"
   │  ├─ [Task] "移动到玩家位置"
   │  └─ [Task] "近战攻击"
   └─ [Sequence] (巡逻)
      ├─ [Task] "选择随机路径点"
      └─ [Task] "移动至路径点"
   ```
3. **动态响应**：当玩家进入视野时，`CanSeePlayer`被设为True，触发追击分支。

---

### **常见问题解决**
- **Q：Blackboard数据不同步？**
  - 确保键名拼写一致，检查数据更新时机（如使用`Event Tick`可能引发性能问题）

- **Q：Decorator条件不触发？**
  - 开启调试工具，确认Blackboard值是否符合预期
  - 检查Decorator的`Observer Aborts`设置（是否允许中断当前行为）

- **Q：行为树卡死？**
  - 避免无限循环（如总返回`Running`的Task）
  - 使用`Force Instanced`模式确保节点状态独立

---

行为树和黑板是UE5 AI系统的灵魂组件，熟练掌握后可以实现从简单NPC到BOSS级复杂AI的所有需求。建议结合官方示例项目（如《Lyra》或《Action RPG》）深入实践！



## 工程实践

### 添加黑板和行为树

这里我们在蓝图里新建一个黑板蓝图，在骷髅兵文件夹下新建一个骷髅兵行为树。

在黑板里我们新增一个目标变量

![image-20250223195852344](.\image-20250223195852344.png)

然后行为树我们添加一个简单的移动行为，让他向着我们的目标移动，移动接受范围设置150

![image-20250223200032820](.\image-20250223200032820.png)

### 在AI控制器中运行行为树

行为树和黑板设置完毕后，现在就需要让我们的骷髅兵执行该行为。回到骷髅兵的AI控制器，我们需要重写`OnPossess`函数。通过`RunBehaviorTree`节点运行行为树。

![image-20250223200446275](.\image-20250223200446275.png)

### 设置黑板键的值

然后下一步就是设置我们的移动目标`TargetActor`黑板键。这里我们就用AI控制器里视觉感知目标作为我们的行为树移动目标

```c++
void AArcaneAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && Actor)
	{
		if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsObject(FName("TargetActor"), Actor);
		}
	}
}

```

### 添加导航网格

此外，因为Detour Crowd Avoidance（绕道人群避障）是需要基于导航网格运作的，所以我们也需要在地图场景里添加导航。添加好后按P可以查看导航网格工作范围（绿色）

![image-20250223201732234](.\image-20250223201732234.png)

自此，我们就能实现简单的AI跟随了。



# 配置AI避障（Configure AI Avoidance）

现在我们的AI已经可以实现绕道避障了，为了看清楚算法正在做什么，我们可以在游戏启动后，通过控制台开启可视化调试：

`ai.crowd.DebugSelectedActors 1`，在我们按下回车后，发现什么都没发生，这是因为我们需要在关卡选择一个代理才能显示调试信息，我们按下`F8`,然后鼠标点击你想查看的避障角色（选择一个骷髅兵）,就可以看到该单位的避障算法的绘制了。

![image-20250223202804014](.\image-20250223202804014.png)

再按一下`F8`就可以关闭选择，进行正常查看

![image-20250223202926625](.\image-20250223202926625.png)

![image-20250223203121468](.\image-20250223203121468.png)

从调试图中我们可以看到算法如何对我们代理的角色周围的环境进行采样，以及路径如何生成。

为了获得更好的导航效果，我们可以在AIController进行更多细致化的设置，比如设置是否启用人群避障，设置人群避让的质量，设置避让组，设置避让半径等。所以回到控制器基类中，我们可以添加这些设置项作为我们的成员变量进行控制。

```c++
private:
	UPROPERTY(EditDefaultsOnly, Category="Detor Crowd Avoidance Config")
	bool bEnableCrowdAvoidance = true;	// 是否启用人群避让

	// meta = (EditCondition = "bEnableCrowdAvoidance")) 意味着只有当 bEnableCrowdAvoidance 为 true 时，才会显示这个属性（即只有在启用人群避让时，才会显示这个属性，才能进行该项设置）
	// , UIMin = "1", UIMax = "4" 则是设置了该属性的最小值和最大值，让我能在编辑器中可以像滑动条一样调整这个整数值，进而控制人群避让的质量
	UPROPERTY(EditDefaultsOnly, Category="Detor Crowd Avoidance Config", meta = (EditCondition = "bEnableCrowdAvoidance", UIMin = "1", UIMax = "4"))
	int32 DetourCrowdAvoidanceQuality = 4;	// 人群避让质量

	UPROPERTY(EditDefaultsOnly, Category="Detor Crowd Avoidance Config", meta = (EditCondition = "bEnableCrowdAvoidance"))
	float CollisionQueryRange = 600.0f;		// 人群避让范围
	
```

然后在BeginPlay中，获取路径跟随组件，进行路径跟随设置

```C++
void AArcaneAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* CrowdFollowingComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		// 是否启用人群避让
		CrowdFollowingComp->SetCrowdSimulationState(bEnableCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::ObstacleOnly);

		// 设置人群避让质量
		switch (DetourCrowdAvoidanceQuality)
		{
		case 1:
			CrowdFollowingComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);
			break;
		case 2:
			CrowdFollowingComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);
			break;
		case 3:
			CrowdFollowingComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
			break;
		case 4:
			CrowdFollowingComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
			break;
		default: break;
		}

		// 设置避让组：1表示我们当前的AI应该对其他AI（EnemyAI，因为我们此前设置的团队ID 1 就是我们的EnemyTeam)进行避让
		CrowdFollowingComp->SetAvoidanceGroup(1);				// 设置避让组
		CrowdFollowingComp->SetGroupsToAvoid(1);		// 设置需要避让的组

		// 设置人群避让半径
		CrowdFollowingComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
		
	}
}
```

## 启用或者关闭人群避让

![image-20250223210037464](.\image-20250223210037464.png)

在启用人群避让，避让质量设置为最高，避让半径600，避让效果如下

![image-20250223210247493](.\image-20250223210247493.png)

在关闭人群避让后

![image-20250223210507614](.\image-20250223210507614.png)

![image-20250223210547309](.\image-20250223210547309.png)

可以看到我们规划避障路径用的权重点位就消失了，自然就失去了人群绕道功能。

## 避让质量

我们将避让质量改为最低，查看避让效果

![image-20250223210748099](.\image-20250223210748099.png)

![image-20250223210833008](.\image-20250223210833008.png)

可以看到，避让质量控制的就是我们避障规划里检测点位的数量，数量越多，就能规划处更细致的路径点位。即算法使用的环境采样数量。

## 碰撞查询半径

然后我们修改一下避让半径，改为100查看

![image-20250223211036163](.\image-20250223211036163.png)

![image-20250223211119201](.\image-20250223211119201.png)

可以看到避让半径改为100后（红色调试圈），碰撞查询范围就明显变小了，查询范围变小，就意味着AI在避让人群的时候，只能选择该范围（红色圈）内的避让点，范围变小，可选择的避让点就少，避让效果自然就会变差。

![image-20250223212404561](.\image-20250223212404561.png)

![image-20250223212630930](.\image-20250223212630930.png)

从这张图可以看到，远处这个骷髅兵在寻路的时候，因为视野被前面两个骷髅兵遮挡，导致这个骷髅兵一直在试图旋转视野，查找新的路径，从而卡在两个骷髅兵后面，这在我们的战斗力是不想看到的，这与一些其他的避让参数调整有关。

打开项目设置，找到集群管理器（CrowdManager）

![image-20250223213158970](.\image-20250223213158970.png)

1. 最大代理半径（MaxAgentRadius）默认为100，这意味着我们可以添加到避让组的代理最大半径是100，在我们添加更多的AI单位的时候，这个值显然将不再够用，这里我们可以改为300.

2. 第二个我们需要调整的是分隔目录限制（SeparationDirClamp），这个值决定了分离力的方向，即在避让的时候，这股力量是应该将代理的角色推向左边还是右边，相邻处于后方时限制到左/右的分离力（Foward和DirToNei之间的点，-1表示禁用）。默认该值为-1，表示禁用该推力，这里我们可以把值改为0.

3. 下一个我们设置的项目是结算碰撞（ResolveCollisons），集群模拟是否解析代理之间的碰撞？如果设置为否，这将由其自己的运动组件处理。这里我们可以勾选该选项.

   ![image-20250223214239987](.\image-20250223214239987.png)

4. 最后一个我们需要调整的变量在回避配置这个数组里（AvoidanceConfig），可以看到它有4个元素，这4个元素分别对应4种不同的回避质量（低，中等，较好，高质量）。

   ![image-20250223214549757](.\image-20250223214549757.png)

   因为这里我们默认使用的是最高质量的避让，所以这里只修改第4个元素。

   ​	我们修改速度偏差（VelocityBias）：它默认是0.5，意味着我们将使用视野原始速度的一半，以及我们当前的跟随组件提供的速度的一半。如果我们将其改为1，就意味着我们将完全使用原始的的速度，而不采用一点跟随组件速度，这就像是我们禁用了我们的回避算法，开启游戏打开调试，我们可以看到避障算法不再对环境进行采样。

   ![image-20250223215428086](.\image-20250223215428086.png)

   反之，如果我们完全将该值设置为0，这就意味着我们完全使用避让组件的速度而不考虑运动组件自己的行走规划。

   ![image-20250223215747864](.\image-20250223215747864.png)

   这样我们AI就不再会出现被其他AI卡住的问题了

   ![image-20250223220307737](.\image-20250223220307737.png)



# 行为树节点类型

![image-20250223220549409](.\image-20250223220549409.png)

目前，我们的EnemyAI只是简单让Enemy移动到玩家前面，我们需要将Enemy分散排布在玩家周围。要实现这样的效果，我们就需要添加更多行为树节点。

#### **关键节点类型**

| 节点类型      | 功能描述                                                     |
| ------------- | ------------------------------------------------------------ |
| **Root**      | 行为树的起点，所有逻辑从Root开始执行                         |
| **Selector**  | 按顺序执行子节点，直到某个子节点成功（类似逻辑“或”）         |
| **Sequence**  | 按顺序执行子节点，直到某个子节点失败（类似逻辑“与”）         |
| **Task**      | 具体行为（如移动、攻击、等待）                               |
| **Decorator** | 附加在节点上的条件判断（如“是否看到玩家？”），决定是否允许执行子节点 |
| **Service**   | 后台持续运行的任务（如定期更新目标位置），不影响主流程但提供数据支持 |

#### 

在 Unreal Engine 5 的行为树中，**Selector**、**Sequence**、**Task**、**Service** 和 **Decorator** 是构建复杂 AI 逻辑的核心节点。它们各自有不同的功能和应用场景，以下是它们的详细解析：

---

### **1. Selector（选择器）**
#### **功能**
- **逻辑行为**：按顺序执行子节点，直到某个子节点返回 **`Success`**。
- **核心思想**：实现“优先级选择”，类似于编程中的 **`if-else`** 或 **逻辑“或”**。

#### **执行流程**
1. 从左到右依次执行子节点。
2. 如果某个子节点返回 `Success`，则 Selector 立即返回 `Success`，停止后续子节点的执行。
3. 如果所有子节点均返回 `Failure`，则 Selector 返回 `Failure`。

#### **典型应用场景**
- **敌人行为决策**：  
  ```text
  [Selector]
  ├─ [攻击玩家]（如果玩家在攻击范围内）
  ├─ [追击玩家]（如果玩家可见）
  └─ [巡逻]（默认行为）
  ```
- **状态优先级**：处理紧急事件（如“血量低时逃跑”优先于其他行为）。

#### **示例代码（伪逻辑）**
```cpp
// 伪代码逻辑
bool Execute() {
    for (Child in Children) {
        if (Child.Execute() == Success) {
            return Success;
        }
    }
    return Failure;
}
```

---

### **2. Sequence（序列）**
#### **功能**
- **逻辑行为**：按顺序执行子节点，直到某个子节点返回 **`Failure`**。
- **核心思想**：实现“顺序执行所有步骤”，类似于编程中的 **`and`** 或 **逻辑“与”**。

#### **执行流程**
1. 从左到右依次执行子节点。
2. 如果某个子节点返回 `Failure`，则 Sequence 立即返回 `Failure`，停止后续执行。
3. 如果所有子节点均返回 `Success`，则 Sequence 返回 `Success`。

#### **典型应用场景**
- **多步骤任务**：  
  ```text
  [Sequence]
  ├─ [移动到目标位置]
  ├─ [播放开启动画]
  └─ [激活机关]
  ```
- **依赖条件**：必须按顺序完成前置任务（如“开门”后才能“进入房间”）。

#### **示例代码（伪逻辑）**
```cpp
bool Execute() {
    for (Child in Children) {
        if (Child.Execute() == Failure) {
            return Failure;
        }
    }
    return Success;
}
```

---

### **3. Task（任务）**
#### **功能**
- **具体行为**：执行一个具体的动作，如移动、攻击、播放动画等。
- **执行状态**：返回 `Success`、`Failure` 或 `Running`（表示任务仍在执行中）。

#### **常见内置任务**
| 任务类型             | 功能描述                         |
| -------------------- | -------------------------------- |
| `Move To`            | 移动到 Blackboard 指定的目标位置 |
| `Wait`               | 等待指定时间                     |
| `Play Animation`     | 播放动画                         |
| `Find Nearest Enemy` | 搜索最近的敌人并写入 Blackboard  |

#### **自定义任务**
可以通过继承 `BTTaskNode`（C++）或创建 `Blueprint Task` 实现自定义逻辑，例如：
```cpp
// C++ 示例：自定义攻击任务
UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
    AICharacter->Attack();
    return EBTNodeResult::Succeeded;
}
```

#### **执行状态说明**
- **`Success`**：任务完成（如成功到达目标位置）。
- **`Failure`**：任务失败（如路径被阻挡）。
- **`Running`**：任务进行中（需持续更新，如移动中）。

---

### **4. Service（服务）**
#### **功能**
- **后台逻辑**：附加在 **Composite 节点**（如 Selector 或 Sequence）上，周期性执行。
- **核心用途**：更新 Blackboard 数据或检测环境变化，不影响主逻辑流程。

#### **执行特点**
- **触发时机**：当父节点（如 Selector/Sequence）处于 `Running` 状态时，按设定的 `Interval` 周期执行。
- **无直接返回值**：仅用于数据更新，不参与行为树的主决策流程。

#### **典型应用场景**
- **环境检测**：  
  ```text
  [Service] "检测玩家是否在视野内"（每0.5秒更新 `CanSeePlayer`）
  ```
- **数据更新**：  
  ```text
  [Service] "更新弹药数量"（每1秒读取背包数据到 `CurrentAmmo`）
  ```

#### **示例代码（伪逻辑）**
```cpp
// C++ 示例：检测玩家是否可见
void UBTService_CheckPlayerVisibility::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
    if (AICharacter->CanSeePlayer()) {
        Blackboard->SetValueAsBool("CanSeePlayer", true);
    } else {
        Blackboard->SetValueAsBool("CanSeePlayer", false);
    }
}
```

---

### **5. Decorator（装饰器）**
#### **功能**
- **条件判断**：附加在节点上，决定是否允许执行该节点或其子节点。
- **核心用途**：实现条件分支（如“是否看到玩家？”或“血量是否低于30%？”）。

#### **常见类型**
| 装饰器类型              | 功能描述                         |
| ----------------------- | -------------------------------- |
| `Blackboard`            | 检查 Blackboard 键值是否符合条件 |
| `Cooldown`              | 添加冷却时间，限制节点执行频率   |
| `Loop`                  | 循环执行子节点                   |
| `Force Success/Failure` | 强制返回成功或失败，用于调试     |

#### **执行模式**
- **`Observer Aborts`** 选项：  
  - `None`：仅在节点启动时检查一次条件。  
  - `Self`：如果条件变为不满足，中断当前节点。  
  - `Lower Priority`：如果条件变为满足，中断低优先级分支。

#### **典型应用场景**
```text
[Selector]
├─ [Sequence]（攻击）
│  └─ [Decorator] "HasAmmo == true"
│     └─ [Task] "射击"
└─ [Sequence]（装弹）
   └─ [Task] "装弹动作"
```

---

### **节点对比总结**
| **节点类型**  | **角色**     | **返回值**              | **执行特点**                 | **典型场景**       |
| ------------- | ------------ | ----------------------- | ---------------------------- | ------------------ |
| **Selector**  | 决策分支     | Success/Failure         | 选择第一个成功的子节点       | 优先级行为选择     |
| **Sequence**  | 顺序执行     | Success/Failure         | 要求所有子节点成功           | 多步骤任务链       |
| **Task**      | 具体动作     | Success/Failure/Running | 执行实际行为（如移动、攻击） | 所有具体行为实现   |
| **Service**   | 后台数据更新 | 无                      | 周期性运行，不阻塞主流程     | 环境检测、数据同步 |
| **Decorator** | 条件门控     | 允许/禁止               | 控制节点是否可执行           | 条件判断、冷却限制 |

---

### **最佳实践与常见问题**
#### **1. 避免过度嵌套**
- 复杂逻辑可以拆分为 **子行为树（Subtree）**，提高可维护性。
- 示例：将“战斗逻辑”和“巡逻逻辑”拆分为独立子树。

#### **2. 合理使用 `Running` 状态**
- 长时间 `Running` 的任务（如移动）需确保能被外部事件中断（如通过 Decorator）。

#### **3. 性能优化**
- **Service** 的 `Interval` 不宜过短（避免高频检测）。
- 对非活跃 AI 禁用行为树（通过 `StopBehaviorTree`）。

#### **4. 调试技巧**
- 使用 `AI.Debug.BT.ShowDebug 1` 可视化行为树执行路径。
- 通过 `Force Success/Failure` 装饰器快速测试分支逻辑。

---

### **实际案例：敌人AI巡逻与追击**
```text
[Selector]
├─ [Sequence]（追击玩家）
│  ├─ [Decorator] "CanSeePlayer == true"
│  ├─ [Task] "移动到玩家位置"
│  └─ [Task] "近战攻击"
│       └─ [Service] "每0.2秒检测玩家距离"（更新 `IsInAttackRange`）
└─ [Sequence]（巡逻）
   ├─ [Task] "选择随机路径点"
   └─ [Task] "移动到路径点"
       └─ [Service] "每5秒重新选择路径点"
```

---

掌握这些节点的用法后，你可以设计出从简单巡逻到复杂团队协作的各类 AI 行为。如果需要更深入的实现细节，建议参考 UE5 官方文档中的 **《Behavior Tree User Guide》** 或分析示例项目（如 **《Lyra Starter Game》**）中的 AI 实现。



## Serverce GetDistToTarget （获取到目标点的距离）

在蓝图里，我们创建一个新的服务，用于获取到目标点的距离。对于服务的创建，一些简单的服务类型我们直接在蓝图里创建，对于一些计算密集型需求，我们采用C++进行创建。

![image-20250223222907039](.\image-20250223222907039.png)

然后在服务中重写ReceiveTickAI事件，计算到目标点的距离，然后将结果设置到黑板中，以供后续行为树使用。

![image-20250223223256792](.\image-20250223223256792.png)



## 观察终止

![image-20250223224629411](.\image-20250223224629411.png)

如图，当前我们简单构建了一个这样的行为树，AI获取到目标的距离，然后判定是否小于等于600，如果是，则等待5s，然后判定是否离得过远，即与玩家之间大于600，如果是，向着玩家移动。

现在，我们需要指定，左边这个分支何时可以终止右侧分支？这样的规则定义需要放在这个装饰器节点下的流程控制里。即，我们需要为`我是否在射击范围内`这个通知观察者填写适当的值。

### 1：观察者通知(Notify Observer)

![image-20250223225455671](.\image-20250223225455671.png)

我们在此通知观察着时失败的选项决定了何时应该开始终止后续流程。这里它有两个策略可以选择：

1. 结果改变时（OnResultChange），结果变化意味着当条件检查结果发生变化时（true->false或者false->true），我应该开始终止分支。对于当前语境来说，我们在这里检查的条件是这个状态目标的值是否小于或者等于600，所以，这通常就是我们想要检查的东西。
2. 值改变时（OnValueChange），值变化的意思就是观察到的黑板键值变化的时候，对于当前语境就是AI距离玩家的距离变化的时候，对于当前键（距离）值变化检查没有太大的意义，我们只关心当前距离是否小于等于600这个临界值这个状态，而对于他值是多少并不关心，所以这里通知策略我们选择观察结果改变时较好。



![image-20250223225534016](.\image-20250223225534016.png)

在我们指定了观察者通知策略后，接下来就是指定观察器终止策略。这里定义了当观察通知的时候要终止哪个分支。它有4个选项

1. 空（None），这个比较简单，即当条件检查结果发生变化时，不会终止任何分支，继续执行后续流程
2. 自己（Self），该选项将使得本身分支在结果变为假的时候终止当前分支，带入语境，即当玩家距离超过600的时候就会停止当前分支的后续流程。即当玩家距离超过600时，就不应该继续wait执行，而应该终止当前分支，让行为进入下一个分支（在这里就是让流程进入向玩家移动的分支）。
3. 低优先级（LowerPriority），这个选项意味着当结果变为真的时候，将终止低优先级的分支，带入当前语境，即当玩家距离小于等于600时终止向玩家移动的分支（低优先级就是在当前分支右边的分支），解释就是当AI移动进入距离玩家600单位以内时，就表明已经进入了AI的射程，就不再需要继续向着玩家移动了，所以右侧的（低优先级）向玩家移动的分支也应该终止。
4. 两个都要（Both），意味着我们将做上述两件事。

现在，我们需要问的问题是，当玩家距离大于600时，我们是否需要立即停止射击？答案显而易见，我们需要停止，因为玩家已经走出射程外了。此时AI应该停止射击或者侧移，转而追踪玩家。

另外一个问题就是，如果距离小于等于600，我们是否希望AI立即开始侧移（绕着玩家攻击移动）。答案也是肯定的，所以当距离小于等于这个值时，AI应该开始侧身移动。

所以，就是为什么这里我们需要选择Both。



# 定位目标角色

![image-20250223233114246](.\image-20250223233114246.png)

如图，我们给射击范围内的行为分支新增了一个侧身移动的序列。对于这个侧身移动，我们首先要做的事情是将AI旋转面向玩家（目标角色）。我们需要他平滑旋转，这就意味着我们需要每帧都更新他的旋转，即我们需要在当前序列添加一个新的服务，因为该服务需要每帧进行，所以这里我们通过C++创建服务。

![image-20250223233633095](.\image-20250223233633095.png)

在创建C++类的时候，我们可以选择很多BTService基类，比如最基础BTService，选择这个，我们将从0开始，需要做很多事情，一个比较好的选择是选择BTService_BlackboardBase，但是这里需要注意，我们千万不能选择从BTService_BlueprintBase继承，因为这个类仅仅被用于蓝图资产子类化，这里，我们选择从最基础开始，选BTService创建我们的服务。

1. 对于一个行为树服务，我们可以设置的第一件事就是这个节点的名称，该名称是属于该父节点类的一个变量（NodeName）
2. 第二个就是服务节点的描述文本，即StaticDescription成员变量。通过设置该变量，我们可以自定义在蓝图服务中想要看到的描述文本。
3. 接下来就是我们需要指定的黑板键，我希望能在服务节点的蓝图细节面板的下拉菜单中可以有供我们选择使用的黑板键。为了实现这点，我们需要在代码里创建`FBlackboardKeySelector`类型变量。

```c++
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_OrientToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UBTService_OrientToTargetActor : public UBTService
{
	GENERATED_BODY()

	UBTService_OrientToTargetActor();

	// UBTNode interface begin
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;	// 从资产初始化
	virtual FString GetStaticDescription() const override;				// 获取静态描述
	// ~UBTNode interface end

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Target")
	FBlackboardKeySelector InTargetActorKey;	// 目标角色键选择器

	UPROPERTY(EditAnywhere, Category="Target")
	float RotationInterpSpeed;	// 旋转插值速度

public:
	
};
```



```c++
// INVI_1998 All Rights Reserved.


#include "AI/BTService_OrientToTargetActor.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_OrientToTargetActor::UBTService_OrientToTargetActor()
{
	NodeName = TEXT("朝向目标旋转");	// 设置节点名称：本地旋转到目标角色

	INIT_SERVICE_NODE_NOTIFY_FLAGS();	// 初始化服务节点通知标志

	RotationInterpSpeed = 5.f;		// 旋转插值速度
	Interval = 0.0f;				// 间隔：因为我们需要每帧都旋转，所以这里设置为0
	RandomDeviation = 0.0f;			// 随机偏差：因为我们不需要随机偏差，所以这里设置为0

	// 通过这个函数来添加一个对象过滤器，这个过滤器可以让我们在下拉菜单中保留我们需要的键的类型
	// GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass() 这个函数的第一个参数是我们的类，第二个参数是我们需要过滤的键，第三个参数是我们需要过滤的类型
	// 这里的意思是：我们需要过滤的是 AActor 类型的键，我们的类是 ThisClass，我们需要过滤的键是 InTargetActorKey
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

void UBTService_OrientToTargetActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	// 在这个函数中，我们需要手动解析我们的黑板键选择器
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InTargetActorKey.ResolveSelectedKey(*BBAsset);	// 解析选择的键
	}
}

FString UBTService_OrientToTargetActor::GetStaticDescription() const
{
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Orient rotation to %s key %s"), *KeyDescription, *GetStaticServiceDescription());
}

void UBTService_OrientToTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 差值旋转
	if (AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName)))
	{
		if (APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn())
		{
			// FRotator TargetRotation = (TargetActor->GetActorLocation() - ControlledPawn->GetActorLocation()).Rotation();
			// ControlledPawn->SetActorRotation(FMath::RInterpTo(ControlledPawn->GetActorRotation(), TargetRotation, DeltaSeconds, RotationInterpSpeed));

			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());
			ControlledPawn->SetActorRotation(FMath::RInterpTo(ControlledPawn->GetActorRotation(), LookAtRotation, DeltaSeconds, RotationInterpSpeed));
		}
	}
}

```

然后回到蓝图，给我的行为树添加朝向目标旋转的服务

![image-20250224002213827](.\image-20250224002213827.png)



# 环境查询系统（EQS-Environment Query System）

对于AI侧身移动，现在我们实现了AI朝向角色旋转，下一步就是为我们的AI选择一个侧身位置，而不是向之前的所有AI全都位于角色前面一排。这就需要我们用到EQS。

在 Unreal Engine 5（UE5）中，**环境查询系统（Environment Query System, EQS）** 是一种强大的AI决策工具，允许AI通过**动态环境分析**选择最优行动路径或位置（如寻找最佳掩体、安全撤离点或资源采集点）。以下是其核心机制和实际应用的全面解析：

---

### **一、EQS 的核心概念**
#### **1. 基本目标**
- **环境感知**：通过一系列**测试（Tests）** 评估场景中的点、Actor或对象的适用性。
- **数据驱动决策**：将环境数据（如距离、视线、威胁值）量化为可比较的分数，供AI选择最优解。

#### **2. 核心组件**
| 组件          | 功能描述                                                     |
| ------------- | ------------------------------------------------------------ |
| **EQS Query** | 定义环境查询的逻辑，包含生成器（Generators）和测试（Tests）的组合 |
| **Generator** | 生成待评估的候选位置或对象（如导航网格点、特定Actor周围的随机点） |
| **Test**      | 对每个候选点进行评分（如距离、安全性、视线遮挡）             |
| **Context**   | 提供查询所需的上下文数据（如AI自身位置、目标Actor引用），为我们的生成器提供参考点 |

---

### **二、EQS 的工作流程**
#### **1. 执行步骤**
1. **生成候选点**：通过Generator生成待评估的位置（如导航网格上的所有可行走点）。
2. **运行测试**：对每个候选点应用一系列Test，计算综合得分。
3. **选择最优解**：根据得分排序，返回最高分的位置或对象。

#### **2. 数据传递示例**
```text
[示例：寻找最佳攻击位置]
Generator → 生成所有可行走点
Test 1 → 距离玩家越近，分数越高（权重30%）
Test 2 → 有掩体遮挡，分数+100（权重50%）
Test 3 → 远离其他敌人，分数+50（权重20%）
最终得分 = (Test1 * 0.3) + (Test2 * 0.5) + (Test3 * 0.2)
```

---

### **三、核心组件详解**
#### **1. Generator（生成器）**
| 类型                     | 功能描述                                                     |
| ------------------------ | ------------------------------------------------------------ |
| **Points: Grid**         | 在指定区域内生成网格点（可调整密度和范围）                   |
| **Points: Pathing Grid** | 基于导航网格（NavMesh）生成可行走点                          |
| **Actors Of Class**      | 生成场景中特定类别的Actor位置（如所有“CoverPoint”实例）      |
| **Composite**            | 组合多个生成器（如先筛选导航网格点，再添加目标周围的随机点） |

#### **2. Test（测试项）**
| 类型              | 功能描述                                                     |
| ----------------- | ------------------------------------------------------------ |
| **Distance**      | 计算候选点与目标的距离（支持“越小越好”或“越大越好”评分）     |
| **Dot Product**   | 评估候选点与目标的朝向关系（如侧翼位置得分更高）             |
| **Overlap**       | 检测候选点是否与其他物体碰撞（如避免选择被障碍物阻挡的位置） |
| **Trace**         | 射线检测（如验证是否有到目标的视线）                         |
| **Gameplay Tags** | 根据Gameplay Tag筛选或评分（如标记“危险区域”的点扣分）       |

#### **3. Context（上下文）**
- **Querier**：发起查询的AI自身（提供位置、旋转、团队等数据）。
- **Target Actor**：目标对象（如玩家角色或资源点）。
- **Custom Blueprint Context**：可通过蓝图扩展自定义数据源。

---

### **四、EQS 在UE5中的实际应用**
#### **1. 典型场景**
- **战术掩体选择**：AI根据玩家位置、掩体遮挡和队友分布选择最佳射击点。
- **资源收集**：寻找最近的资源点，避开敌方巡逻区域。
- **动态撤离路径**：在爆炸或火灾等动态威胁下，实时计算安全路径。

#### **2. 与行为树（Behavior Tree）的集成**
在行为树中通过 **`Run EQS Query`** 任务节点调用EQS查询，结果写入Blackboard供后续节点使用：
```text
[行为树示例]
1. [Task] "Run EQS Query" → 寻找最佳掩体位置
2. [Task] "Move To" → 移动到Blackboard中的`BestCoverPoint`
3. [Task] "Shoot" → 攻击玩家
```

#### **3. 调试与可视化**
- **调试命令**：  
  - 控制台输入 `AI.Debug.EQS 1` 显示当前EQS查询结果。
  - 使用不同颜色区分得分（绿色=高分，红色=低分）。
- **蓝图调试**：在EQS查询编辑器中启用`Debug`模式，逐步查看测试结果。

---

### **五、EQS 配置示例：寻找安全掩体**
#### **1. 创建EQS查询**
1. **Generator**：使用`Points: Pathing Grid`生成导航网格点。
2. **Tests**：
   - **Test 1**：`Distance`（到玩家的距离，越小越好，权重0.2）。
   - **Test 2**：`Trace`（检测是否有到玩家的视线，无视线则+100分，权重0.5）。
   - **Test 3**：`Distance`（到最近爆炸物的距离，越大越好，权重0.3）。

#### **2. 行为树调用**
```cpp
// C++ 示例：在行为树任务中运行EQS
UBTTask_RunEQSQuery::UBTTask_RunEQSQuery() {
    EQSQuery = LoadObject<UEnvQuery>(...); // 加载预定义的EQS资源
    BlackboardKey.AddVectorFilter(this, TEXT("BestCoverLocation"));
}
```

#### **3. 结果应用**
AI根据`BestCoverLocation`移动到掩体后，触发攻击或防御行为。

---

### **六、性能优化与高级技巧**
#### **1. 优化策略**
- **降低查询频率**：避免每帧运行EQS，通过行为树的`Service`控制触发间隔。
- **简化测试项**：优先使用计算量低的测试（如`Distance`比`Trace`高效）。
- **分层查询**：先用粗略的Grid筛选大致区域，再用高精度测试细化选择。

#### **2. 动态权重调整**
通过蓝图动态修改测试权重，应对战场变化：
```cpp
// 示例：当AI血量低于30%时，增加“远离敌人”的权重
EQSQuery->Tests[2].Weight = (AIHealth < 0.3) ? 0.6 : 0.3;
```

#### **3. 自定义测试**
继承`EnvQueryTest`类实现自定义评分逻辑（如根据地形高度打分）：
```cpp
void UEnvQueryTest_CustomHeight::RunTest(FEnvQueryInstance& QueryInstance) {
    for (FEnvQueryItem& Item : QueryInstance.Items) {
        float Z = Item.GetLocation().Z;
        Item.Score = FMath::Clamp(Z / MaxHeight, 0.0f, 1.0f);
    }
}
```

---

### **七、EQS 与其他系统的协作**
| 系统                           | 协作方式                                  |
| ------------------------------ | ----------------------------------------- |
| **导航系统（NavMesh）**        | Generator依赖NavMesh生成可行走点          |
| **AI感知系统（AIPerception）** | 结合感知数据（如玩家位置）作为Context输入 |
| **Gameplay Tags**              | 通过Tag标记区域危险性，供EQS测试筛选      |

---

### **八、常见问题与解决方案**
| **问题**           | **解决方案**                                       |
| ------------------ | -------------------------------------------------- |
| EQS返回无效位置    | 检查Generator是否基于NavMesh，Test是否过滤了所有点 |
| 查询性能开销大     | 减少Grid密度，使用`Actors Of Class`替代全局搜索    |
| 测试权重配置不合理 | 通过调试可视化验证单项得分，调整权重比例           |

---

### **九、EQS 的局限性**
- **静态环境依赖**：若场景动态变化频繁（如可破坏建筑），需高频更新查询。
- **主观性配置**：测试权重需要人工调参，可能影响AI行为合理性。
- **复杂逻辑限制**：超复杂决策需结合行为树分层和黑板数据。

---

EQS 是UE5中实现智能环境响应的核心工具，尤其适合需要动态权衡多种因素的AI决策场景。结合官方示例（如 **《ShooterGame》** 中的AI掩体选择）实践，可以快速掌握其高级用法。



## 自定义查询上下文

![image-20250224011321490](.\image-20250224011321490.png)

这里，我们的生成器的Context（上下文）选项是**Querier**：发起查询的AI自身，生成的环境查询结果就是，在AI周围生成一堆查询点，这与我们想要的环境查询是不一致的，我们想要的是基于玩家位置发起查询，查询玩家周围一个合适的点位用于AI的侧身移动点位。所以，默认提供的这些上下文就不再满足需求，这里我们需要自定义上下文。

![image-20250224011817968](.\image-20250224011817968.png)

因为需求比较简单，这里我们直接在蓝图里创建该上下文。然后我们重写ProvideSingleActor函数，通过黑板组件返回玩家角色作为我们的Context

![image-20250224012316424](.\image-20250224012316424.png)

但是，如果直接将上述Context应用到EQS中，我们就会发现，无法成功进行环境查询，因为此时我们还是在编辑器中，并没有实际运行游戏，所以是拿不到玩家对象的。（正常我们测试EQS都是在编辑器中测试）。所以我们可以在编辑器环境下，获取玩家出生点作为玩家对象进行测试，如下：

![image-20250224013000691](.\image-20250224013000691.png)



![image-20250224013828532](.\image-20250224013828532.png)

这就是我们用于生成AI侧身移动的点位的EQS，首先我们在玩家周围生成圈项目点，然后引入第一个测试，即丢弃掉那些AI不可达的点（因为环境查询的结果是要作为AI寻路的目标点的，路径不可达自然要丢弃），然后在剩下的点中，排除掉玩家200距离内的点，我们不希望AI直接导航到玩家脸上，然后排除掉距离玩家800距离的点，同样，距离过远的点我们也排除，剩下的点就可以按得分作为我们AI侧移的点位了。

![image-20250224014429447](.\image-20250224014429447.png)

然后我们将环境查询添加到行为树中，同时将结果我们不取最优值，我们取25%以内的最佳得分，这样随机化更高一点。然后将结果赋值给一个黑板键（vector向量值），最后让我们的AI移动到该点位。

![image-20250224014810495](.\image-20250224014810495.png)



我们目前的EQS在玩家和AI都靠近墙壁的时候，会有一个问题，即EQS会把墙壁边缘的点位都聚拢统计，导致出现怪物聚集的问题，这也是我们不想看到的，所以需要继续在EQS中添加测试。

![image-20250224015614154](.\image-20250224015614154.png)

这里我们在EQS中再添加一条测试，剔除距离玩家480距离内的点，这样，边缘聚拢的点就将被踢出。

![image-20250224015940353](.\image-20250224015940353.png)

![image-20250224020110816](.\image-20250224020110816.png)

![image-20250224020419939](.\image-20250224020419939.png)

![image-20250224020655667](.\image-20250224020655667.png)



# BehaviorTree Task

## 切换侧移状态

上述过程我们已经实现了AI查找并移动到侧身移动的点位，现在，是时候实现侧身移动了。这里就需要我们通过Task实现。

我们先创建一个Enemy基类Task蓝图。

![image-20250224111901087](.\image-20250224111901087.png)

![image-20250224111327708](.\image-20250224111327708.png)

对于该基类蓝图，我们只做两件基础的事情，第一，重载ReceiveExecuteAI事件，将其控制的Pawn转为有效的Enemy角色并缓存起来。第二：新建一个执行任务的空函数，基类里我们什么都不做，具体干什么交给子类蓝图去实现，然后调用，然后结束节点执行。

> 记住一件事，对于任何自定义蓝图行为树节点，无论是行为树，任务装饰器，或者服务，他们默认情况下都是世界中的实例，这意味着变量的值可以在不同执行之间保存。但是这只适用于蓝图情况，而不适用于C++的情况。比如在自定义C++行为树中任务重，变量的值无法保存，这是因为它还没有被实例化，这时候它还只是一个C++类。
>
> ### **核心概念：节点的“实例化”**
>
> - **默认行为**：
>   当你在蓝图中创建一个自定义的行为树节点（无论是任务 Task、装饰器 Decorator 还是服务 Service），默认情况下，该节点在行为树运行时会**为每个使用它的AI实例单独创建一个副本**。这意味着：
>   - 每个AI（如敌人、NPC）拥有自己独立的节点实例。
>   - 节点中定义的**变量值在多次执行中会被保留**（例如，一个计时器变量不会在每次重新进入节点时重置）。
> - **类比解释**：
>   可以将其想象为每个AI都有一份“私有笔记本”，记录该节点的状态。即使多个AI同时执行同一行为树，它们的节点变量互不影响。
>
> ### **行为树节点的生命周期**
>
> - **实例化时机**：
>
>   - 当行为树启动时（如调用`RunBehaviorTree`），所有自定义节点会为当前AI控制器创建实例。
>   - 节点实例的生命周期与AI控制器绑定，行为树停止时销毁。
>
> - **变量持久性示例**：
>   假设你有一个自定义任务 `BTTask_CountAttempts`，其中包含一个变量 `Attempts`：
>
>   ```C++
>   // 伪代码示例
>   class BTTask_CountAttempts : public BTTaskNode {
>       int32 Attempts = 0; // 该变量会被保留
>       virtual EBTNodeResult::Type ExecuteTask() override {
>           Attempts++;
>           return EBTNodeResult::Succeeded;
>       }
>   };
>   ```
>
>   - 第一次执行时，`Attempts = 1`；第二次执行时，`Attempts = 2`，以此类推。

然后我们创建一个基于该基类行为树任务的子类行为树任务蓝图，用以实现我们具体的AI侧身环绕移动，该蓝图简而言之就是判定是否需要侧身移动，需要就禁用AI角色移动组件的旋转朝向移动，然后根据需要设置侧身移动状态下的最大移动速度，然后给角色添加一个状态Tag，用于后续动画层调用侧身移动动画。如果不需要就设置回角色的旋转朝向移动，然后依据情况设置回角色的默认最大行走速度，移除状态Tag。

![image-20250224121049362](.\image-20250224121049362.png)

注意：这里如果对行走速度设置有需求，我们不应该在任务里保存这个最大行走速度值，因为在任务里你获取到的角色组件的最大行走速度有可能已经被改动过了，我们可以在AIController里，在运行行为树后立即保存当前角色的默认最大行走速度。

![image-20250224121719475](.\image-20250224121719475.png)

然后就是将该任务添加到行为树执行流程中。

![image-20250224121907353](.\image-20250224121907353.png)



## 计算侧身移动方向

现在我们可以改变它的侧身行走速度，并在状态启用时为其添加一个游戏标签。

因为要实现角色的侧身移动，我们的混合空间就不再是1维驱动了，除了基础的行走速度，还应该加入行走方向，所以该混合空间应该是一个2D的混合空间。所以我们需要在基类里添加这个行走方向的计算。

```c++
UCLASS()
class ARCANEGLYPH_API UArcaneCharacterAnimInstance : public UArcaneBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	// 该动画实例的更新函数是线程安全的，运行在独立的工作线程中，而非游戏线程中，因此可以在该函数中进行一些计算密集型的操作
	// 这意味着使用这个函数可以提高动画的性能，使用该函数来计算我们需要的动画数据是一个很大的优化项
	// 但是需要注意的是，该函数中不能访问任何非线程安全的数据，比如 Actor 的成员变量等
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<AArcaneCharacterBase> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;	// 地面速度

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;	// 是否有加速度

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;	// 角色运动方向与面朝方向之间的水平平面角度差

private:
	
};
```

然后，计算该角度差，我们可以使用UE自己的函数库UKismetAnimationLibrary，记得需要添加模块`AnimGraphRuntime`。同时对于动画里这种牵扯到计算的，我们一般都放入到单独的线程函数里计算，以提高性能。

```c++
void UArcaneCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!IsValid(OwnerCharacter) || !IsValid(OwnerCharacterMovementComponent))
	{
		return;
	}

	GroundSpeed = OwnerCharacterMovementComponent->Velocity.Size2D();
	bHasAcceleration = OwnerCharacterMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.0f;

	// 计算当前移动的方向
	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetActorRotation());
}
```

下面是代码里用到的`UKismetAnimationLibrary::CalculateDirection`函数原型和实现。

```C++
	/** 
	 * Returns degree of the angle between Velocity and Rotation forward vector
	 * The range of return will be from [-180, 180]. Useful for feeding directional blendspaces.
	 * @param	Velocity		The velocity to use as direction relative to BaseRotation
	 * @param	BaseRotation	The base rotation, e.g. of a pawn
	 */
	UFUNCTION(BlueprintPure, Category="Animation|Utilities")
	static ANIMGRAPHRUNTIME_API float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation);
```

函数实现

```c++
float UKismetAnimationLibrary::CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation)
{
	if (!Velocity.IsNearlyZero())
	{
		const FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		const FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		const FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		const FVector NormalizedVel = Velocity.GetSafeNormal2D();

		// get a cos(alpha) of forward vector vs velocity
		const float ForwardCosAngle = static_cast<float>(FVector::DotProduct(ForwardVector, NormalizedVel));
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		const float RightCosAngle = static_cast<float>(FVector::DotProduct(RightVector, NormalizedVel));
		if (RightCosAngle < 0.f)
		{
			ForwardDeltaDegree *= -1.f;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}
```



在 Unreal Engine 中，`UKismetAnimationLibrary::CalculateDirection` 函数用于计算角色运动方向与面朝方向之间的水平平面角度差。该函数在动画系统中非常关键，常用于驱动角色的移动混合空间（Blend Space），实现不同方向动画的平滑过渡。以下是对该函数的详细解析：

---

### **函数功能**
#### **输入参数**
- **`Velocity`**：角色的速度向量（世界坐标系），表示当前移动方向。
- **`BaseRotation`**：角色的旋转（如Pawn的朝向），用于确定“前向”基准方向。

#### **返回值**
- **角度范围**：`[-180°, 180°]`，表示速度方向与角色面朝方向的水平偏差角。
  - **正角度**：速度方向在角色右侧（右转）。
  - **负角度**：速度方向在角色左侧（左转）。
  - **0°**：速度方向与面朝方向一致。

#### **核心用途**
- **动画混合**：将角度值输入到Blend Space，根据方向切换行走、奔跑、转身动画。
- **AI行为**：判断移动方向是否需要调整角色朝向（如追击目标时修正路径）。

---

### **实现步骤解析**
```cpp
float UKismetAnimationLibrary::CalculateDirection(
    const FVector& Velocity, 
    const FRotator& BaseRotation
) {
    // 1. 若速度接近零，直接返回0（无需计算）
    if (!Velocity.IsNearlyZero()) {
        // 2. 将BaseRotation转换为旋转矩阵，提取前向和右向向量
        const FMatrix RotMatrix = FRotationMatrix(BaseRotation);
        const FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X); // 前向（X轴）
        const FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);   // 右向（Y轴）

        // 3. 归一化速度向量，并投影到水平平面（忽略Z轴）
        const FVector NormalizedVel = Velocity.GetSafeNormal2D();

        // 4. 计算前向向量与速度的夹角（余弦值）
        const float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
        float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

        // 5. 通过右向向量判断方向正负
        const float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);
        if (RightCosAngle < 0.f) {
            ForwardDeltaDegree *= -1.f; // 速度在左侧时角度为负
        }

        return ForwardDeltaDegree;
    }
    return 0.f; // 无速度时返回0
}
```

---

### **关键步骤详解**
#### **1. 速度为零的快捷处理**
- **逻辑**：若速度接近零（`Velocity.IsNearlyZero()`），直接返回0°，避免无意义计算。
- **意义**：角色静止时，默认方向与面朝方向一致。

#### **2. 旋转矩阵与轴向提取**
- **`FRotationMatrix(BaseRotation)`**：将角色的旋转转换为旋转矩阵。
- **`GetScaledAxis`**：提取矩阵的X轴（前向）和Y轴（右向）单位向量。
  - 例如，若角色面朝正北，则：
    - `ForwardVector = (1, 0, 0)`（假设X轴为前）
    - `RightVector = (0, 1, 0)`（Y轴为右）

#### **3. 速度归一化与投影**
- **`GetSafeNormal2D()`**：将速度向量归一化，并投影到水平平面（忽略Z轴高度）。
  - 避免垂直方向（如跳跃、坠落）影响水平方向判断。

#### **4. 计算前向夹角**
- **点积公式**：  
  $$
  \cos(\theta) = \frac{\text{ForwardVector} \cdot \text{NormalizedVel}}{|\text{ForwardVector}| \cdot |\text{NormalizedVel}|}
  $$
  
  - 由于向量已归一化，点积直接为余弦值。
- **反余弦转换**：通过`FMath::Acos`得到弧度值，再转为角度。

#### **5. 方向正负判断**
- **右向点积**：计算速度向量与右向向量的点积（`RightCosAngle`）。
  - **`RightCosAngle < 0`**：表示速度方向在角色左侧（相对于前向和右向构成的平面）。
  - 此时将角度设为负值，确保最终范围为`[-180°, 180°]`。

---

### **几何示意图**
```
                ForwardVector (X轴)
                   ↑
                   | θ
          Left     |     Right
(-θ) <------------o------------> (+θ)
                   | 
                   | 
        RightVector (Y轴)
```

- **θ**：通过前向向量与速度向量计算得到的基础角度（0°~180°）。
- **正负号**：由右向向量点积决定，左侧为负，右侧为正。

---

### **实际应用示例**
#### **场景1：角色向前移动**
- **速度向量**：`(100, 0, 0)`（与前向一致）
- **计算过程**：
  - `ForwardCosAngle = 1.0` → `θ = 0°`
  - `RightCosAngle = 0.0` → 不调整符号
- **返回值**：`0°`

#### **场景2：角色向右移动**
- **速度向量**：`(0, 100, 0)`（与右向一致）
- **计算过程**：
  - `ForwardCosAngle = 0.0` → `θ = 90°`
  - `RightCosAngle = 1.0` → 符号不变
- **返回值**：`90°`

#### **场景3：角色向左后方移动**
- **速度向量**：`(-50, -50, 0)`（标准化后`(-0.707, -0.707, 0)`）
- **计算过程**：
  - `ForwardCosAngle = -0.707` → `θ = 135°`
  - `RightCosAngle = -0.707 < 0` → 角度设为`-135°`
- **返回值**：`-135°`

---

### **常见问题与优化**
#### **1. 为什么忽略Z轴？**
- **目的**：角色移动动画通常仅关注水平方向（如行走、奔跑），垂直运动（跳跃、坠落）由其他动画状态处理。

#### **2. 如何处理速度向量为零？**
- **设计**：直接返回0°，避免因零向量归一化导致的除零错误（`GetSafeNormal2D`已隐含保护）。

#### **3. 性能优化**
- **计算轻量**：仅涉及向量运算和一次反三角函数，适合每帧调用。
- **替代方案**：若需更高性能，可预计算旋转矩阵或缓存方向向量。

#### **4. 扩展三维方向**
若需包含垂直方向的角度计算，可修改`GetSafeNormal2D()`为`GetSafeNormal()`，并调整投影逻辑。

---

### **在动画蓝图中的应用**
#### **Blend Space 配置**
1. 在动画蓝图中调用`CalculateDirection`，传入角色的速度和旋转。
2. 将返回值连接到Blend Space的横轴（如`Direction`）。
3. 根据角度值混合不同方向的动画（如前进、后退、左右转身）。

```cpp
// 示例：在动画蓝图中获取方向角度
float Direction = UKismetAnimationLibrary::CalculateDirection(
    GetVelocity(), 
    GetActorRotation()
);
```

#### **调试技巧**
- **可视化调试**：在角色身上绘制速度向量（蓝色）和前向向量（绿色），直观观察角度关系。
- **控制台命令**：`showdebug animation` 查看实时角度值。

---

### **总结**
`CalculateDirection` 函数通过向量运算和几何分析，将复杂的运动方向关系转化为直观的角度值，是UE动画系统中实现动态方向响应的核心工具。理解其实现细节后，开发者可以更灵活地定制角色移动动画，或将其逻辑扩展至AI决策、物理交互等场景。



![image-20250224124413828](.\image-20250224124413828.png)

![image-20250224132334480](.\image-20250224132334480.png)



## 计算攻击欲望

现在，我们的AI已经可以环绕角色进行侧身移动了，接下来就是处理AI的攻击逻辑。

首先我们需要计算攻击概率，以此来中断我们的行为树分支（停止侧身移动，开始攻击）。

所以，我们可以为我们的攻击序列创建一个装饰器，该装饰器用于计算攻击概率，计算结果将作为攻击序列是否执行的重要依据。

![image-20250224143233817](.\image-20250224143233817.png)

这里我们就使用`RandomBoolWithWeight`节点的结果返回装饰器是否成功（是否想要攻击）。该节点用指定权重获取随机几率，权重范围在0-1之间，例如，权重0.6，返回成功的概率就有60%。这样我们就能通过该装饰器实现AI攻击欲望的控制。

![image-20250224143726729](.\image-20250224143726729.png)



## EQS添加点积测试（点积常用来求向量夹角）

现在我们的EQS测试结果，在和玩家距离很近的时候，会出现最终选择的点位会穿过我们的角色。如下：

![image-20250224192945450](.\image-20250224192945450.png)

所以，我们现在需要修改EQS查询，使其剔除AI点到目标点连线会和玩家相交的点位。

但是，其实这样游戏效果也不是很完美，因为，此时AI会向着玩家移动，移动到玩家背后的点位，这在逻辑上也有点奇怪，最好的剔除效果是，利用AI的前向方向向量和AI到点位的连线做点积，排除夹角小于阈值的点位。如下

![image-20250224201204561](.\image-20250224201204561.png)

同时，我们这里测试模式（点积计算）不必使用3D向量进行计算，因为我们只需要考虑XY平面下的角度即可。

![image-20250224200704993](.\image-20250224200704993.png)



# MotionWarping

在我们实现了AI的攻击逻辑后，我们已经可以让AI进行基础挥砍攻击了，为了让攻击逻辑更加合理，我们添加了MotionWarping，让AI能在攻击的时候向我们的玩家，从而不至于对着空气挥砍。

![image-20250225122725616](.\image-20250225122725616.png)

因为玩家会在游戏过程中一直移动，仅仅在动画蒙太奇中指定扭曲动画提供者是不行的，所以MotionWarping的运作需要程序员指定面朝的对象，但是同时我们也不应该每帧都更新攻击目标，因为这也不是必要的，所以我们选择在行为树中添加行为树服务来做这个事情。

![image-20250225123429052](.\image-20250225123429052.png)

然后，将其添加到行为树中。

![image-20250225123547465](.\image-20250225123547465.png)

此时测试我们会发现，因为MotionWarping是在攻击动画蒙太奇中做的，这就意味着，我们的AI是一边攻击一边旋转，对于攻击有前摇的动画，我们可以缩短这个MotionWarping状态条，缓解这个问题，但是，对于一些前摇没那么长的动画，这个攻击效果就不会很好，有什么解决办法？

我们可以在攻击行为之前，就让AI面向我们的玩家，这样就能解决AI后续的扭曲幅度过大的问题。在行为树中，有这样一个原生任务节点`Rotate to face BB entry`，顾名思义，它可以让我们的行为树拥有者(AI)面向我们指定的对象。我们在攻击任务前添加这个任务：

![image-20250225124257759](.\image-20250225124257759.png)

但是，测试的时候，我们发现，在需要他转向的时候，行为树会卡在这个任务节点上

![image-20250225124503787](.\image-20250225124503787.png)

这是因为`Rotate to face BB entry`节点它实际调整拥有的Pawn的旋转是通过调整该Pawn的控制器的旋转进行控制的：

```c++
EBTNodeResult::Type UBTTask_RotateToFaceBBEntry::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController == NULL || AIController->GetPawn() == NULL)
	{
		return EBTNodeResult::Failed;
	}

	...
				AIController->SetFocus(ActorValue, EAIFocusPriority::Gameplay);
	...
				AIController->SetFocalPoint(KeyValue, EAIFocusPriority::Gameplay);
	...
				const FVector FocalPoint = PawnLocation + DirectionVector * 10000.0f;
				// set focal somewhere far in the indicated direction
				AIController->SetFocalPoint(FocalPoint, EAIFocusPriority::Gameplay);
...

	return Result;
}
```



而就是这一点，导致在我们目前的案例里正好冲突。我们的AI角色，因为需要实现侧身移动效果，所以在其角色基类中，我们对其的旋转控制如下：

```c++
GetCharacterMovement()->bOrientRotationToMovement = true;		// 角色面向移动方向
GetCharacterMovement()->bUseControllerDesiredRotation = false;		// 不使用控制器期望旋转
```

该节点只有在Pawn设置为了使用控制器期望的旋转时，它才能正常工作。即如下：

```c++
GetCharacterMovement()->bOrientRotationToMovement = false;       // 角色面向移动方向
GetCharacterMovement()->bUseControllerDesiredRotation = true;      // 使用控制器期望旋转
```

可是这样直接修改之后，又会影响到我们的侧身移动，所以，我们不能简单的直接使用该节点，而需要做些自定义的修改。



## 构建原生 BT 任务

为了解决上述问题，这里我们可以基于最基础的Task节点`UBTTaskNode`，建立我们的自定义任务类。



