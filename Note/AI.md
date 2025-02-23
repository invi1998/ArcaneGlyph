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
