// INVI_1998 All Rights Reserved.


#include "ArcaneGameplayTags.h"

namespace ArcaneGameplayTags
{
	// Input Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "角色移动");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "InputTag.Look", "角色观察");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "角色跳跃");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Roll, "InputTag.Roll", "角色翻滚");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SwitchTarget, "InputTag.SwitchTarget", "切换目标");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_EquipSword, "InputTag.EquipSword", "装备剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_EquipTwinblades, "InputTag.EquipTwinblades", "装备双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UnEquipSword, "InputTag.UnEquipSword", "卸下剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UnEquipTwinblades, "InputTag.UnEquipTwinblades", "卸下双刃剑");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LightAttack_Sword, "InputTag.LightAttack.Sword", "轻攻击:剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LightAttack_Twinblades, "InputTag.LightAttack.Twinblades", "轻攻击:双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_HeavyAttack_Sword, "InputTag.HeavyAttack.Sword", "重攻击:剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_HeavyAttack_Twinblades, "InputTag.HeavyAttack.Twinblades", "重攻击:双刃剑");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_PickUp_HealSoul, "InputTag.PickUp.HealSoul", "拾取疗伤灵魂");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_PickUp_RageSoul, "InputTag.PickUp.RageSoul", "拾取愤怒灵魂");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UseItem_HealPotion, "InputTag.UseItem.HealPotion", "使用药水");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UseItem_RagePotion, "InputTag.UseItem.RagePotion", "使用愤怒药水");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SpecialWeaponAbility_Light, "InputTag.SpecialWeaponAbility.Light", "特殊武器技能:轻攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SpecialWeaponAbility_Heavy, "InputTag.SpecialWeaponAbility.Heavy", "特殊武器技能:重攻击");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_MustBeHeld, "InputTag.MustBeHeld", "必须被持续按住");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_MustBeHeld_Block, "InputTag.MustBeHeld.Block", "必须被持续格挡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_MustBeHeld_Shift, "InputTag.MustBeHeld.Shift", "必须被持续冲刺");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Toggleable, "InputTag.Toggleable", "可切换");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Toggleable_TargetLock, "InputTag.Toggleable.TargetLock", "可切换目标锁定");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Toggleable_Rage, "InputTag.Toggleable.Rage", "可切换愤怒");

	// Player Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Weapon_Sword, "Player.Weapon.Sword", "玩家武器：剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Weapon_Twinblades, "Player.Weapon.Twinblades", "玩家武器：双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Weapon_MageStaff, "Player.Weapon.MageStaff", "玩家武器：法杖");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Equipped_Sword, "Player.Event.Equipped.Sword", "玩家事件：装备剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_UnEquipped_Sword, "Player.Event.UnEquipped.Sword", "玩家事件：卸下剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Equipped_Twinblades, "Player.Event.Equipped.Twinblades", "玩家事件：装备双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_UnEquipped_Twinblades, "Player.Event.UnEquipped.Twinblades", "玩家事件：卸下双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_ComboChangeHandRight, "Player.Event.ComboChange.HandRight", "玩家事件：连击切换-右手");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_ComboChangeHandLeft, "Player.Event.ComboChange.HandLeft", "玩家事件：连击切换-左手");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_BeginRotateSlot, "Player.Event.BeginRotateSlot", "玩家事件：开始旋转槽");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_EndRotateSlot, "Player.Event.EndRotateSlot", "玩家事件：结束旋转槽");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_HitPause, "Player.Event.HitPause", "玩家事件：受击暂停");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_BlockSuccess, "Player.Event.BlockSuccess", "玩家事件：格挡成功");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_BlockFailed, "Player.Event.BlockFailed", "玩家事件：格挡失败");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_SwitchLockTarget_Left, "Player.Event.SwitchLockTarget.Left", "玩家事件：切换锁定目标-向左切换");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_SwitchLockTarget_Right, "Player.Event.SwitchLockTarget.Right", "玩家事件：切换锁定目标-向右切换");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_RollSuccess, "Player.Event.RollSuccess", "玩家事件：翻滚成功");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_ActivateRage, "Player.Event.ActivateRage", "玩家事件：激活愤怒");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_StopAttackingBackswing, "Player.Event.StopAttackingBackswing", "玩家事件：停止攻击后摇");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_PickupSoul_Start, "Player.Event.PickupSoul.Start", "玩家事件：拾取灵魂开始");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_PickupSoul_End, "Player.Event.PickupSoul.End", "玩家事件：拾取灵魂结束");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_UsePotion, "Player.Event.UsePotion", "玩家事件：使用药水");
	
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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_TargetLock, "Player.Ability.TargetLock", "玩家技能：目标锁定");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Rage, "Player.Ability.Rage", "玩家技能：愤怒");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_SpecialWeaponAbility_Light, "Player.Ability.SpecialWeaponAbility.Light", "玩家技能：特殊武器技能：轻攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_SpecialWeaponAbility_Heavy, "Player.Ability.SpecialWeaponAbility.Heavy", "玩家技能：特殊武器技能：重攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Pickup_Soul_Heal, "Player.Ability.PickupSoul.Heal", "玩家技能：拾取疗伤灵魂");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Pickup_Soul_Rage, "Player.Ability.PickupSoul.Rage", "玩家技能：拾取愤怒灵魂");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_UseItem_HealPotion, "Player.Ability.UseItem.HealPotion", "玩家技能：使用药水");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_UseItem_RagePotion, "Player.Ability.UseItem.RagePotion", "玩家技能：使用愤怒药水");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Cooldown_Ability_Block, "Player.Cooldown.Ability.Block", "玩家冷却：技能：格挡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Cooldown_Ability_Rage, "Player.Cooldown.Ability.Rage", "玩家冷却：技能：愤怒");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Cooldown_SpecialWeaponAbility_Light, "Player.Cooldown.SpecialWeaponAbility.Light", "玩家冷却：特殊武器技能：轻攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Cooldown_SpecialWeaponAbility_Heavy, "Player.Cooldown.SpecialWeaponAbility.Heavy", "玩家冷却：特殊武器技能：重攻击");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_JumpToFinish, "Player.Status.JumpToFinish", "玩家状态：连招跳转到结束");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Rolling, "Player.Status.Rolling", "玩家状态：翻滚中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Blocking, "Player.Status.Blocking", "玩家状态：格挡中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Shifting, "Player.Status.Shifting", "玩家状态：冲刺中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_TargetLocking, "Player.Status.TargetLocking", "玩家状态：目标锁定中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Rage_Activating, "Player.Status.Rage.Activating", "玩家状态：愤怒激活中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Rage_Active, "Player.Status.Rage.Active", "玩家状态：愤怒激活");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Rage_Full, "Player.Status.Rage.Full", "玩家状态：愤怒满");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Rage_None, "Player.Status.Rage.None", "玩家状态：愤怒无");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_SetByCaller_AttackType_Light, "Player.SetByCaller.AttackType.Light", "玩家设置：攻击类型-轻");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_SetByCaller_AttackType_Heavy, "Player.SetByCaller.AttackType.Heavy", "玩家设置：攻击类型-重");

	// Enemy Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Weapon_VanterDual, "Enemy.Weapon.VanterDual", "敌人武器：双头戟");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Weapon_SkeletonSword, "Enemy.Weapon.SkeletonSword", "敌人武器：骷髅剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Weapon_DualEdgedClaws_Left, "Enemy.Weapon.DualEdgedClaws.Left", "敌人武器：双刃爪-左");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Weapon_DualEdgedClaws_Right, "Enemy.Weapon.DualEdgedClaws.Right", "敌人武器：双刃爪-右");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Ability_Melee, "Enemy.Ability.Melee", "敌人技能：近战攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Ability_Range, "Enemy.Ability.Range", "敌人技能：远程攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Ability_SummonEnemy, "Enemy.Ability.SummonEnemy", "敌人技能：召唤敌人");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Ability_Skill, "Enemy.Ability.Skill", "敌人技能：技能");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Ability_SpawnSoul, "Enemy.Ability.SpawnSoul", "敌人技能：生成灵魂");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Ability_Block, "Enemy.Ability.Block", "敌人技能：格挡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Ability_Dodge, "Enemy.Ability.Dodge", "敌人技能：闪避");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Event_SummonEnemy, "Enemy.Event.SummonEnemy", "敌人事件：召唤敌人");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Event_SuccessBlock, "Enemy.Event.SuccessBlock", "敌人事件：格挡成功");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Event_FailedBlock, "Enemy.Event.FailedBlock", "敌人事件：格挡失败");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Status_Strafing, "Enemy.Status.Strafing", "敌人状态：侧身环绕移动");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Status_UnderAttack, "Enemy.Status.UnderAttack", "敌人状态：受到攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Status_Unblockable, "Enemy.Status.Unblockable", "敌人状态：无法格挡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy_Status_Blocking, "Enemy.Status.Blocking", "敌人状态：格挡中");

	// Shared Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_MeleeAttack_1, "Shared.Event.MeleeAttack.1", "共享事件：近战攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_MeleeAttack_2, "Shared.Event.MeleeAttack.2", "共享事件：近战攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_HitReact, "Shared.Event.HitReact", "共享事件：受击反应");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_SpawnProjectile, "Shared.Event.SpawnProjectile", "共享事件：生成投射物");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_AOE, "Shared.Event.AOE", "共享事件：群体伤害");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_FellDown, "Shared.Event.FellDown", "共享事件：倒地");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Ability_HitReact, "Shared.Ability.HitReact", "共享技能：受击反应");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Ability_Death, "Shared.Ability.Death", "共享技能：死亡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Ability_FellDown, "Shared.Ability.FellDown", "共享技能：倒地");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_SetByCaller_BaseDamage, "Shared.SetByCaller.BaseDamage", "共享：基础伤害");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_SetByCaller_HealPercentage, "Shared.SetByCaller.HealPercentage", "共享：治疗百分比");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_SetByCaller_RageRecoverPercentage, "Shared.SetByCaller.RageRecoverPercentage", "共享：愤怒恢复百分比");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_Dead, "Shared.Status.Dead", "共享状态：死亡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_HitReact_Front, "Shared.Status.HitReact.Front", "共享状态：受击反应-前");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_HitReact_Left, "Shared.Status.HitReact.Left", "共享状态：受击反应-左");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_HitReact_Right, "Shared.Status.HitReact.Right", "共享状态：受击反应-右");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_HitReact_Back, "Shared.Status.HitReact.Back", "共享状态：受击反应-后");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_Invincibility, "Shared.Status.Invincibility", "共享状态：无敌状态");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_FellDown, "Shared.Status.FellDown", "共享状态：倒地");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_Hegemony, "Shared.Status.Hegemony", "共享状态：霸体");
	
}
