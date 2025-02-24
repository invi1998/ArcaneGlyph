// INVI_1998 All Rights Reserved.


#include "ArcaneGameplayTags.h"

namespace ArcaneGameplayTags
{
	// Input Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "角色移动");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "InputTag.Look", "角色观察");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "角色跳跃");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_EquipSword, "InputTag.EquipSword", "装备剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UnEquipSword, "InputTag.UnEquipSword", "卸下剑");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LightAttack_Sword, "InputTag.LightAttack.Sword", "轻攻击:剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_HeavyAttack_Sword, "InputTag.HeavyAttack.Sword", "重攻击:剑");

	// Player Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Weapon_Sword, "Player.Weapon.Sword", "玩家武器：剑");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Equipped_Sword, "Player.Event.Equipped.Sword", "玩家事件：装备剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_UnEquipped_Sword, "Player.Event.UnEquipped.Sword", "玩家事件：卸下剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_ComboChangeHandRight, "Player.Event.ComboChange.HandRight", "玩家事件：连击切换-右手");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_ComboChangeHandLeft, "Player.Event.ComboChange.HandLeft", "玩家事件：连击切换-左手");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_BeginRotateSlot, "Player.Event.BeginRotateSlot", "玩家事件：开始旋转槽");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_EndRotateSlot, "Player.Event.EndRotateSlot", "玩家事件：结束旋转槽");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_HitPause, "Player.Event.HitPause", "玩家事件：受击暂停");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Equip_Sword, "Player.Ability.Equip.Sword", "玩家技能：装备剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_UnEquip_Sword, "Player.Ability.UnEquip.Sword", "玩家技能：卸下剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Attack_Light_Sword, "Player.Ability.Attack.Light.Sword", "玩家技能：轻攻击-剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Attack_Heavy_Sword, "Player.Ability.Attack.Heavy.Sword", "玩家技能：重攻击-剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_HitPause, "Player.Ability.HitPause", "玩家技能：受击暂停");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_JumpToFinish, "Player.Status.JumpToFinish", "玩家状态：连招跳转到结束");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_SetByCaller_AttackType_Light, "Player.SetByCaller.AttackType.Light", "玩家设置：攻击类型-轻");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_SetByCaller_AttackType_Heavy, "Player.SetByCaller.AttackType.Heavy", "玩家设置：攻击类型-重");

	// Enemy Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Weapon_VanterDual, "Enemy.Weapon.VanterDual", "敌人武器：双头戟");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Weapon_SkeletonSword, "Enemy.Weapon.SkeletonSword", "敌人武器：骷髅剑");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Ability_Melee, "Enemy.Ability.Melee", "敌人技能：近战攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Ability_Range, "Enemy.Ability.Range", "敌人技能：远程攻击");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Status_Strafing, "Enemy.Status.Strafing", "敌人状态：侧身环绕移动");

	// Shared Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_MeleeAttack, "Shared.Event.MeleeAttack", "共享事件：近战攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_HitReact, "Shared.Event.HitReact", "共享事件：受击反应");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Ability_HitReact, "Shared.Ability.HitReact", "共享技能：受击反应");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Ability_Death, "Shared.Ability.Death", "共享技能：死亡");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_SetByCaller_BaseDamage, "Shared.SetByCaller.BaseDamage", "共享：基础伤害");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_Dead, "Shared.Status.Dead", "共享状态：死亡");
	
}
