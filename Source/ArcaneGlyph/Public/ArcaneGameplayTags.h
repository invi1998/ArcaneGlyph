// INVI_1998 All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace ArcaneGameplayTags
{
	// Input Tags
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);		// 输入标签：移动
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);		// 输入标签：观察
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump);		// 输入标签：跳跃
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Roll);		// 输入标签：翻滚

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipSword);		// 输入标签：装备剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipTwinblades);		// 输入标签：装备双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnEquipSword);		// 输入标签：卸下剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnEquipTwinblades);		// 输入标签：卸下双刃剑

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_Sword);		// 输入标签：轻攻击：剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_Twinblades);		// 输入标签：轻攻击：双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_Sword);		// 输入标签：重攻击：剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_Twinblades);		// 输入标签：重攻击：双刃剑
  
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MustBeHeld);		// 输入标签：必须被持续按住
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MustBeHeld_Block);		// 输入标签：必须被持续格挡
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MustBeHeld_Shift);		// 输入标签：必须被持续冲刺
	
	// Player Tags
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Sword);		// 玩家武器：剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Twinblades);		// 玩家武器：双刃剑

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equipped_Sword);		// 玩家事件：装备剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_UnEquipped_Sword);		// 玩家事件：卸下剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equipped_Twinblades);		// 玩家事件：装备双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_UnEquipped_Twinblades);		// 玩家事件：卸下双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_ComboChangeHandRight);		// 玩家事件：连击切换-右手
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_ComboChangeHandLeft);		// 玩家事件：连击切换-左手
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_BeginRotateSlot);		// 玩家事件：开始旋转槽
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_EndRotateSlot);			// 玩家事件：结束旋转槽
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_HitPause);				// 玩家事件：受击暂停

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Sword);			// 玩家技能：装备剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_UnEquip_Sword);		// 玩家技能：卸下剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Twinblades);	// 玩家技能：装备双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_UnEquip_Twinblades);	// 玩家技能：卸下双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Sword);	// 玩家技能：轻攻击-剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_Sword);	// 玩家技能：重攻击-剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Twinblades);	// 玩家技能：轻攻击-双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_Twinblades);	// 玩家技能：重攻击-双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_HitPause);			// 玩家技能：受击暂停
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Roll);				// 玩家技能：翻滚
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Block);				// 玩家技能：格挡
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Shift);				// 玩家技能：冲刺

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_JumpToFinish);		// 玩家状态：连招跳转到结束
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rolling);			// 玩家状态：翻滚中
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Blocking);		// 玩家状态：格挡中
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Shifting);		// 玩家状态：冲刺中
	

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Light);	// 玩家设置：攻击类型-轻
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Heavy);	// 玩家设置：攻击类型-重

	// Enemy Tags
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon_VanterDual);		// 敌人武器：双头戟
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon_SkeletonSword);			// 敌人武器：骷髅剑
	
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Melee);		// 敌人技能：近战攻击
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Range);		// 敌人技能：远程攻击

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Strafing);		// 敌人状态：侧身环绕移动
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_UnderAttack);		// 敌人状态：受到攻击
	
	// Shared Tags
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeAttack);		// 共享事件：近战攻击
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_HitReact);			// 共享事件：受击反应

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_HitReact);		// 共享技能：受击反应
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_Death);			// 共享技能：死亡

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_BaseDamage);		// 共享：基础伤害

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Dead);					// 共享状态：死亡
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Front);		// 共享状态：受击反应-前
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Left);		// 共享状态：受击反应-左
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Right);		// 共享状态：受击反应-右
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Back);		// 共享状态：受击反应-后
	
}

