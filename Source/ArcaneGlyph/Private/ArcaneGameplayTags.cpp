// INVI_1998 All Rights Reserved.


#include "ArcaneGameplayTags.h"

namespace ArcaneGameplayTags
{
	// Input Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "角色移动");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "InputTag.Look", "角色观察");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "角色跳跃");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Roll, "InputTag.Roll", "角色翻滚");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_EquipSword, "InputTag.EquipSword", "装备剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_EquipTwinblades, "InputTag.EquipTwinblades", "装备双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UnEquipSword, "InputTag.UnEquipSword", "卸下剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UnEquipTwinblades, "InputTag.UnEquipTwinblades", "卸下双刃剑");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LightAttack_Sword, "InputTag.LightAttack.Sword", "轻攻击:剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LightAttack_Twinblades, "InputTag.LightAttack.Twinblades", "轻攻击:双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_HeavyAttack_Sword, "InputTag.HeavyAttack.Sword", "重攻击:剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_HeavyAttack_Twinblades, "InputTag.HeavyAttack.Twinblades", "重攻击:双刃剑");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_MustBeHeld, "InputTag.MustBeHeld", "必须被持续按住");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_MustBeHeld_Block, "InputTag.MustBeHeld.Block", "必须被持续格挡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_MustBeHeld_Shift, "InputTag.MustBeHeld.Shift", "必须被持续冲刺");

	// Player Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Weapon_Sword, "Player.Weapon.Sword", "玩家武器：剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Weapon_Twinblades, "Player.Weapon.Twinblades", "玩家武器：双刃剑");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Equipped_Sword, "Player.Event.Equipped.Sword", "玩家事件：装备剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_UnEquipped_Sword, "Player.Event.UnEquipped.Sword", "玩家事件：卸下剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Equipped_Twinblades, "Player.Event.Equipped.Twinblades", "玩家事件：装备双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_UnEquipped_Twinblades, "Player.Event.UnEquipped.Twinblades", "玩家事件：卸下双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_ComboChangeHandRight, "Player.Event.ComboChange.HandRight", "玩家事件：连击切换-右手");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_ComboChangeHandLeft, "Player.Event.ComboChange.HandLeft", "玩家事件：连击切换-左手");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_BeginRotateSlot, "Player.Event.BeginRotateSlot", "玩家事件：开始旋转槽");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_EndRotateSlot, "Player.Event.EndRotateSlot", "玩家事件：结束旋转槽");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_HitPause, "Player.Event.HitPause", "玩家事件：受击暂停");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Equip_Sword, "Player.Ability.Equip.Sword", "玩家技能：装备剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_UnEquip_Sword, "Player.Ability.UnEquip.Sword", "玩家技能：卸下剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Equip_Twinblades, "Player.Ability.Equip.Twinblades", "玩家技能：装备双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_UnEquip_Twinblades, "Player.Ability.UnEquip.Twinblades", "玩家技能：卸下双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Attack_Light_Sword, "Player.Ability.Attack.Light.Sword", "玩家技能：轻攻击-剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Attack_Heavy_Sword, "Player.Ability.Attack.Heavy.Sword", "玩家技能：重攻击-剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Attack_Light_Twinblades, "Player.Ability.Attack.Light.Twinblades", "玩家技能：轻攻击-双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Attack_Heavy_Twinblades, "Player.Ability.Attack.Heavy.Twinblades", "玩家技能：重攻击-双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_HitPause, "Player.Ability.HitPause", "玩家技能：受击暂停");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Roll, "Player.Ability.Roll", "玩家技能：翻滚");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Block, "Player.Ability.Block", "玩家技能：格挡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Shift, "Player.Ability.Shift", "玩家技能：冲刺");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_JumpToFinish, "Player.Status.JumpToFinish", "玩家状态：连招跳转到结束");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Rolling, "Player.Status.Rolling", "玩家状态：翻滚中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Blocking, "Player.Status.Blocking", "玩家状态：格挡中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Shifting, "Player.Status.Shifting", "玩家状态：冲刺中");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_SetByCaller_AttackType_Light, "Player.SetByCaller.AttackType.Light", "玩家设置：攻击类型-轻");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_SetByCaller_AttackType_Heavy, "Player.SetByCaller.AttackType.Heavy", "玩家设置：攻击类型-重");

	// Enemy Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Weapon_VanterDual, "Enemy.Weapon.VanterDual", "敌人武器：双头戟");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Weapon_SkeletonSword, "Enemy.Weapon.SkeletonSword", "敌人武器：骷髅剑");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Ability_Melee, "Enemy.Ability.Melee", "敌人技能：近战攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Ability_Range, "Enemy.Ability.Range", "敌人技能：远程攻击");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Status_Strafing, "Enemy.Status.Strafing", "敌人状态：侧身环绕移动");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Status_UnderAttack, "Enemy.Status.UnderAttack", "敌人状态：受到攻击");

	// Shared Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_MeleeAttack, "Shared.Event.MeleeAttack", "共享事件：近战攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_HitReact, "Shared.Event.HitReact", "共享事件：受击反应");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Ability_HitReact, "Shared.Ability.HitReact", "共享技能：受击反应");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Ability_Death, "Shared.Ability.Death", "共享技能：死亡");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_SetByCaller_BaseDamage, "Shared.SetByCaller.BaseDamage", "共享：基础伤害");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_Dead, "Shared.Status.Dead", "共享状态：死亡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_HitReact_Front, "Shared.Status.HitReact.Front", "共享状态：受击反应-前");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_HitReact_Left, "Shared.Status.HitReact.Left", "共享状态：受击反应-左");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_HitReact_Right, "Shared.Status.HitReact.Right", "共享状态：受击反应-右");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_HitReact_Back, "Shared.Status.HitReact.Back", "共享状态：受击反应-后");
	
}
