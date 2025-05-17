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
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SwitchTarget);		// 输入标签：切换目标

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipSword);		// 输入标签：装备剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipTwinblades);		// 输入标签：装备双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnEquipSword);		// 输入标签：卸下剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnEquipTwinblades);		// 输入标签：卸下双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipSpear);		//	输入标签：装备长矛
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnEquipSpear);		//	输入标签：卸下长矛

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_Sword);		// 输入标签：轻攻击：剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_Twinblades);		// 输入标签：轻攻击：双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_Spear);		// 输入标签：轻攻击：长矛
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_Sword);		// 输入标签：重攻击：剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_Twinblades);		// 输入标签：重攻击：双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_Spear);		// 输入标签：重攻击：长矛

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_PickUp_HealSoul);		// 输入标签：拾取疗伤灵魂
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_PickUp_RageSoul);		// 输入标签：拾取愤怒灵魂
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UseItem_HealPotion);		// 输入标签：使用物品：疗伤药水
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UseItem_RagePotion);		// 输入标签：使用物品：愤怒药水

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SpecialWeaponAbility_Light);		// 输入标签：特殊武器技能：轻攻击
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SpecialWeaponAbility_Heavy);		// 输入标签：特殊武器技能：重攻击
  
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MustBeHeld);		// 输入标签：必须被持续按住
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MustBeHeld_Block);		// 输入标签：必须被持续格挡
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MustBeHeld_Shift);		// 输入标签：必须被持续冲刺

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Toggleable);		// 输入标签：可切换
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Toggleable_TargetLock);		// 输入标签：可切换目标锁定
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Toggleable_Rage);		// 输入标签：可切换愤怒
	
	// Player Tags
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Sword);		// 玩家武器：剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Twinblades);		// 玩家武器：双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_MageStaff);		// 玩家武器：法杖
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Spear);		// 玩家武器：长矛

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equipped_Sword);		// 玩家事件：装备剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_UnEquipped_Sword);		// 玩家事件：卸下剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equipped_Twinblades);		// 玩家事件：装备双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_UnEquipped_Twinblades);		// 玩家事件：卸下双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equipped_Spear);		// 玩家事件：装备长矛
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_UnEquipped_Spear);		// 玩家事件：卸下长矛
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_ComboChangeHandRight);		// 玩家事件：连击切换-右手
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_ComboChangeHandLeft);		// 玩家事件：连击切换-左手
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_BeginRotateSlot);		// 玩家事件：开始旋转槽
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_EndRotateSlot);			// 玩家事件：结束旋转槽
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_HitPause);				// 玩家事件：受击暂停
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_BlockSuccess);			// 玩家事件：格挡成功
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_BlockFailed);			// 玩家事件：格挡失败
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_SwitchLockTarget_Left);		// 玩家事件：切换锁定目标-向左切换
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_SwitchLockTarget_Right);		// 玩家事件：切换锁定目标-向右切换
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_RollSuccess);			// 玩家事件：翻滚成功
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_StopAttackingBackswing);		// 玩家事件：停止攻击后摇
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_PickupSoul_Start);			// 玩家事件：拾取灵魂开始
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_PickupSoul_End);			// 玩家事件：拾取灵魂结束
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_UsePotion);				// 玩家事件：使用药水
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_ComboWindow);			// 玩家事件：连击窗口
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_InputBuffer_WindowOpened);	// 玩家事件：输入缓冲窗口打开
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_InputBuffer_WindowClosed);	// 玩家事件：输入缓冲窗口关闭

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Sword);			// 玩家技能：装备剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_UnEquip_Sword);		// 玩家技能：卸下剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Twinblades);	// 玩家技能：装备双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_UnEquip_Twinblades);	// 玩家技能：卸下双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Spear);			// 玩家技能：装备长矛
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_UnEquip_Spear);		// 玩家技能：卸下长矛
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Sword);	// 玩家技能：轻攻击-剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_Sword);	// 玩家技能：重攻击-剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Twinblades);	// 玩家技能：轻攻击-双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_Twinblades);	// 玩家技能：重攻击-双刃剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Spear);	// 玩家技能：轻攻击-长矛
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_Spear);	// 玩家技能：重攻击-长矛
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_HitPause);			// 玩家技能：受击暂停
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Roll);				// 玩家技能：翻滚
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Block);				// 玩家技能：格挡
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Shift);				// 玩家技能：冲刺
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_TargetLock);		// 玩家技能：目标锁定
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Rage);				// 玩家技能：愤怒
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_SpecialWeaponAbility_Light);	// 玩家技能：特殊武器技能：轻攻击
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_SpecialWeaponAbility_Heavy);	// 玩家技能：特殊武器技能：重攻击
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Pickup_Soul_Heal);		// 玩家技能：拾取疗伤灵魂
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Pickup_Soul_Rage);		// 玩家技能：拾取愤怒灵魂
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_UseItem_HealPotion);	// 玩家技能：使用物品：疗伤药水
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_UseItem_RagePotion);	// 玩家技能：使用物品：愤怒药水

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_InputBuffer_Active);	// 玩家技能：输入缓冲窗口激活
	
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Cooldown_Ability_Block);	// 玩家冷却：技能：格挡
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Cooldown_Ability_Rage);	// 玩家冷却：技能：愤怒
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Cooldown_SpecialWeaponAbility_Light);	// 玩家冷却：特殊武器技能：轻攻击
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Cooldown_SpecialWeaponAbility_Heavy);	// 玩家冷却：特殊武器技能：重攻击
	
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_JumpToFinish);		// 玩家状态：连招跳转到结束
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rolling);			// 玩家状态：翻滚中
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Blocking);		// 玩家状态：格挡中
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Shifting);		// 玩家状态：冲刺中
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_TargetLocking);	// 玩家状态：目标锁定中
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rage_Activating);		// 玩家状态：愤怒激活中
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rage_Active);		// 玩家状态：愤怒激活
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rage_Full);		// 玩家状态：愤怒满
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rage_None);		// 玩家状态：愤怒无
	
	
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Light);	// 玩家设置：攻击类型-轻
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Heavy);	// 玩家设置：攻击类型-重

	// Enemy Tags
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon_VanterDual);		// 敌人武器：双头戟
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon_SkeletonSword);			// 敌人武器：骷髅剑
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon_DualEdgedClaws_Left);		// 敌人武器：双刃爪-左
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon_DualEdgedClaws_Right);		// 敌人武器：双刃爪-右
	
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Melee);		// 敌人技能：近战攻击
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Range);		// 敌人技能：远程攻击
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_SummonEnemy);		// 敌人技能：召唤敌人
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Skill);		// 敌人技能：技能
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_SpawnSoul);		// 敌人技能：生成灵魂
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Block);		// 敌人技能：格挡
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Dodge);		// 敌人技能：闪避

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Event_SummonEnemy);		// 敌人事件：召唤敌人
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Event_SuccessBlock);		// 敌人事件：格挡成功
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Event_FailedBlock);		// 敌人事件：格挡失败

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Strafing);		// 敌人状态：侧身环绕移动
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_UnderAttack);		// 敌人状态：受到攻击
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Unblockable);		// 敌人状态：无法格挡
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Blocking);		// 敌人状态：格挡中
	
	// Shared Tags
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeAttack_1);		// 共享事件：近战攻击（Box1）
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeAttack_2);		// 共享事件：近战攻击（Box2）
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_HitReact);			// 共享事件：受击反应
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_SpawnProjectile);	// 共享事件：生成投射物
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_AOE);				// 共享事件：群体伤害
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_FellDown);			// 共享事件：倒地

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_HitReact);		// 共享技能：受击反应
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_Death);			// 共享技能：死亡
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_FellDown);		// 共享技能：倒地

	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_BaseDamage);		// 共享：基础伤害
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_HealPercentage);	// 共享：治疗百分比
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_RageRecoverPercentage);	// 共享：愤怒恢复百分比
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_BaseRageGain);	// 共享：基础愤怒获取
	
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Dead);					// 共享状态：死亡
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Front);		// 共享状态：受击反应-前
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Left);		// 共享状态：受击反应-左
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Right);		// 共享状态：受击反应-右
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Back);		// 共享状态：受击反应-后
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Invincibility);		// 共享状态：无敌状态
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_FellDown);				// 共享状态：受击反应-倒地
	ARCANEGLYPH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Hegemony);				// 共享状态：霸体状态
	
}

