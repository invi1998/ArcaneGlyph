// INVI_1998 All Rights Reserved.


#include "ArcaneGameplayTags.h"

namespace ArcaneGameplayTags
{
	// Input Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_DrawCommonUIHUD, "InputTag.DrawCommonUIHUD", "绘制通用UI HUD");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "角色移动");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "InputTag.Look", "角色观察");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "角色跳跃");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Roll, "InputTag.Roll", "角色翻滚");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SwitchTarget, "InputTag.SwitchTarget", "切换目标");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_EquipSword, "InputTag.EquipSword", "装备剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_EquipTwinblades, "InputTag.EquipTwinblades", "装备双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UnEquipSword, "InputTag.UnEquipSword", "卸下剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UnEquipTwinblades, "InputTag.UnEquipTwinblades", "卸下双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_EquipSpear, "InputTag.EquipSpear", "装备长矛");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UnEquipSpear, "InputTag.UnEquipSpear", "卸下长矛");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LightAttack_Sword, "InputTag.LightAttack.Sword", "轻攻击:剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LightAttack_Twinblades, "InputTag.LightAttack.Twinblades", "轻攻击:双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LightAttack_Spear, "InputTag.LightAttack.Spear", "轻攻击:长矛");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_HeavyAttack_Sword, "InputTag.HeavyAttack.Sword", "重攻击:剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_HeavyAttack_Twinblades, "InputTag.HeavyAttack.Twinblades", "重攻击:双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_HeavyAttack_Spear, "InputTag.HeavyAttack.Spear", "重攻击:长矛");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_JumpAttack_Light, "InputTag.JumpAttack.Light", "跳跃攻击:轻");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_JumpAttack_Heavy, "InputTag.JumpAttack.Heavy", "跳跃攻击:重");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ChangeComboType_VerticalStaff, "InputTag.ChangeComboType.VerticalStaff", "切换连击类型:立棍");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ChangeComboType_HorizontalStaff, "InputTag.ChangeComboType.HorizontalStaff", "切换连击类型:横棍");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ChangeComboType_CleavingStaff, "InputTag.ChangeComboType.CleavingStaff", "切换连击类型:劈棍");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ChangeComboType_ThrustingStaff, "InputTag.ChangeComboType.ThrustingStaff", "切换连击类型:戳棍");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Combo_LightAttack_1, "InputTag.Combo.LightAttack.1", "连击轻攻击1");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Combo_LightAttack_2, "InputTag.Combo.LightAttack.2", "连击轻攻击2");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Combo_LightAttack_3, "InputTag.Combo.LightAttack.3", "连击轻攻击3");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Combo_LightAttack_4, "InputTag.Combo.LightAttack.4", "连击轻攻击4");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Combo_ChangeHand_1, "InputTag.Combo.ChangeHand.1", "切手技-重");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Combo_ChangeHand_2, "InputTag.Combo.ChangeHand.2", "切手技-重-重");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_MustBeHeld_Combo_Heavy, "InputTag.MustBeHeld.Combo.Heavy", "必须被持续按住-重攻击");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_PickUp_HealSoul, "InputTag.PickUp.HealSoul", "拾取疗伤灵魂");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_PickUp_RageSoul, "InputTag.PickUp.RageSoul", "拾取愤怒灵魂");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UseItem_HealPotion, "InputTag.UseItem.HealPotion", "使用药水");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UseItem_RagePotion, "InputTag.UseItem.RagePotion", "使用愤怒药水");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UseItem_ManaPotion, "InputTag.UseItem.ManaPotion", "使用法力药水");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UseItem_ExtraDamagePotion, "InputTag.UseItem.ExtraDamagePotion", "使用额外伤害药水");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UseItem_CurrentItem, "InputTag.UseItem.CurrentItem", "使用当前药水");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SwitchItem_Left, "InputTag.SwitchItem.Left", "切换物品:向左切换");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SwitchItem_Right, "InputTag.SwitchItem.Right", "切换物品:向右切换");
	
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SpecialWeaponAbility_Light, "InputTag.SpecialWeaponAbility.Light", "特殊武器技能:轻攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SpecialWeaponAbility_Heavy, "InputTag.SpecialWeaponAbility.Heavy", "特殊武器技能:重攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SpecialWeaponAbility_Freeze, "InputTag.SpecialWeaponAbility.Freeze", "特殊武器技能:定身");

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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Weapon_Spear, "Player.Weapon.Spear", "玩家武器：长矛");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Equipped_Sword, "Player.Event.Equipped.Sword", "玩家事件：装备剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_UnEquipped_Sword, "Player.Event.UnEquipped.Sword", "玩家事件：卸下剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Equipped_Twinblades, "Player.Event.Equipped.Twinblades", "玩家事件：装备双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_UnEquipped_Twinblades, "Player.Event.UnEquipped.Twinblades", "玩家事件：卸下双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Equipped_Spear, "Player.Event.Equipped.Spear", "玩家事件：装备长矛");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_UnEquipped_Spear, "Player.Event.UnEquipped.Spear", "玩家事件：卸下长矛");
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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_ComboWindow_Opened_Light, "Player.Event.ComboWindow.Opened.Light", "玩家事件：连招窗口打开-轻攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_ComboWindow_Opened_Heavy, "Player.Event.ComboWindow.Opened.Heavy", "玩家事件：连招窗口打开-重攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_ComboWindow_Closed_Light, "Player.Event.ComboWindow.Closed.Light", "玩家事件：连招窗口关闭-轻攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_ComboWindow_Closed_Heavy, "Player.Event.ComboWindow.Closed.Heavy", "玩家事件：连招窗口关闭-重攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_InputBuffer_WindowOpened, "Player.Event.InputBuffer.WindowOpened", "玩家事件：输入窗口打开");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_InputBuffer_WindowClosed, "Player.Event.InputBuffer.WindowClosed", "玩家事件：输入窗口关闭");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Charge_Complete, "Player.Event.Charge.Complete", "玩家事件：蓄力完成");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Charge_Start, "Player.Event.Charge.Start", "玩家事件：蓄力开始");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Charge_EnterLoop, "Player.Event.Charge.EnterLoop", "玩家事件：蓄力进入循环");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Equip_Sword, "Player.Ability.Equip.Sword", "玩家技能：装备剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_UnEquip_Sword, "Player.Ability.UnEquip.Sword", "玩家技能：卸下剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Equip_Twinblades, "Player.Ability.Equip.Twinblades", "玩家技能：装备双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_UnEquip_Twinblades, "Player.Ability.UnEquip.Twinblades", "玩家技能：卸下双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Equip_Spear, "Player.Ability.Equip.Spear", "玩家技能：装备长矛");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_UnEquip_Spear, "Player.Ability.UnEquip.Spear", "玩家技能：卸下长矛");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Attack_Light_Sword, "Player.Ability.Attack.Light.Sword", "玩家技能：轻攻击-剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Attack_Heavy_Sword, "Player.Ability.Attack.Heavy.Sword", "玩家技能：重攻击-剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Attack_Light_Twinblades, "Player.Ability.Attack.Light.Twinblades", "玩家技能：轻攻击-双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Attack_Heavy_Twinblades, "Player.Ability.Attack.Heavy.Twinblades", "玩家技能：重攻击-双刃剑");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Attack_Light_Spear, "Player.Ability.Attack.Light.Spear", "玩家技能：轻攻击-长矛");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Attack_Heavy_Spear, "Player.Ability.Attack.Heavy.Spear", "玩家技能：重攻击-长矛");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_HitPause, "Player.Ability.HitPause", "玩家技能：受击暂停");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Roll, "Player.Ability.Roll", "玩家技能：翻滚");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Block, "Player.Ability.Block", "玩家技能：格挡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Shift, "Player.Ability.Shift", "玩家技能：冲刺");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Jump, "Player.Ability.Jump", "玩家技能：跳跃");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_TargetLock, "Player.Ability.TargetLock", "玩家技能：目标锁定");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Rage, "Player.Ability.Rage", "玩家技能：愤怒");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_SpecialWeaponAbility_Light, "Player.Ability.SpecialWeaponAbility.Light", "玩家技能：特殊武器技能：轻攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_SpecialWeaponAbility_Heavy, "Player.Ability.SpecialWeaponAbility.Heavy", "玩家技能：特殊武器技能：重攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_SpecialWeaponAbility_Freeze, "Player.Ability.SpecialWeaponAbility.Freeze", "玩家技能：特殊武器技能：定身");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Pickup_Soul_Heal, "Player.Ability.PickupSoul.Heal", "玩家技能：拾取疗伤灵魂");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Pickup_Soul_Rage, "Player.Ability.PickupSoul.Rage", "玩家技能：拾取愤怒灵魂");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_UseItem_HealPotion, "Player.Ability.UseItem.HealPotion", "玩家技能：使用药水");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_UseItem_RagePotion, "Player.Ability.UseItem.RagePotion", "玩家技能：使用愤怒药水");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_UseItem_ManaPotion, "Player.Ability.UseItem.ManaPotion", "玩家技能：使用法力药水");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_UseItem_DamagePotion, "Player.Ability.UseItem.DamagePotion", "玩家技能：使用额外伤害药水");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_UseItem_CurrentItem, "Player.Ability.UseItem.CurrentItem", "玩家技能：使用当前物品");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Combo_Light_1, "Player.Ability.Combo.Light.1", "玩家技能：连招：轻攻击-1");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Combo_Light_2, "Player.Ability.Combo.Light.2", "玩家技能：连招：轻攻击-2");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Combo_Light_3, "Player.Ability.Combo.Light.3", "玩家技能：连招：轻攻击-3");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Combo_Light_4, "Player.Ability.Combo.Light.4", "玩家技能：连招：轻攻击-4");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Combo_ChangeHand_1, "Player.Ability.Combo.ChangeHand.1", "玩家技能：连招：一段切手技");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Combo_ChangeHand_2, "Player.Ability.Combo.ChangeHand.2", "玩家技能：连招：二段切手技");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Combo_Heavy, "Player.Ability.Combo.Heavy", "玩家技能：连招：重攻击");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_ChangeComboType_VerticalStaff, "Player.Ability.ChangeComboType.VerticalStaff", "玩家技能：切换连击类型：立棍");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_ChangeComboType_HorizontalStaff, "Player.Ability.ChangeComboType.HorizontalStaff", "玩家技能：切换连击类型：横棍");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_ChangeComboType_CleavingStaff, "Player.Ability.ChangeComboType.CleavingStaff", "玩家技能：切换连击类型：劈棍");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_ChangeComboType_ThrustingStaff, "Player.Ability.ChangeComboType.ThrustingStaff", "玩家技能：切换连击类型：戳棍");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Combo_Light_1, "Player.Combo.Light.1", "玩家连招：轻攻击-1");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Combo_Light_2, "Player.Combo.Light.2", "玩家连招：轻攻击-2");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Combo_Light_3, "Player.Combo.Light.3", "玩家连招：轻攻击-3");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Combo_Light_4, "Player.Combo.Light.4", "玩家连招：轻攻击-4");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Combo_ChangeHand_1, "Player.Combo.ChangeHand.1", "玩家连招：一段切手技");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Combo_ChangeHand_2, "Player.Combo.ChangeHand.2", "玩家连招：二段切手技");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Combo_Heavy, "Player.Combo.Heavy", "玩家连招：重攻击");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_JumpAttack_Light, "Player.JumpAttack.Light", "玩家技能：跳跃攻击：轻");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_JumpAttack_Heavy, "Player.JumpAttack.Heavy", "玩家技能：跳跃攻击：重");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Cooldown_Ability_Block, "Player.Cooldown.Ability.Block", "玩家冷却：技能：格挡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Cooldown_Ability_Rage, "Player.Cooldown.Ability.Rage", "玩家冷却：技能：愤怒");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Cooldown_SpecialWeaponAbility_Light, "Player.Cooldown.SpecialWeaponAbility.Light", "玩家冷却：特殊武器技能：轻攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Cooldown_SpecialWeaponAbility_Heavy, "Player.Cooldown.SpecialWeaponAbility.Heavy", "玩家冷却：特殊武器技能：重攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Cooldown_SpecialWeaponAbility_Freeze, "Player.Cooldown.SpecialWeaponAbility.Freeze", "玩家冷却：特殊武器技能：定身");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_JumpAttack_Activating, "Player.Status.JumpAttack.Activating", "玩家状态：跳跃攻击激活中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Jumping, "Player.Status.Jumping", "玩家状态：跳跃中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_JumpAttacking, "Player.Status.JumpAttacking", "玩家状态：跳跃攻击中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_JumpToFinish, "Player.Status.JumpToFinish", "玩家状态：连招跳转到结束");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Rolling, "Player.Status.Rolling", "玩家状态：翻滚中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Blocking, "Player.Status.Blocking", "玩家状态：格挡中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Shifting, "Player.Status.Shifting", "玩家状态：冲刺中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_TargetLocking, "Player.Status.TargetLocking", "玩家状态：目标锁定中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Rage_Activating, "Player.Status.Rage.Activating", "玩家状态：愤怒激活中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Rage_Active, "Player.Status.Rage.Active", "玩家状态：愤怒激活");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Rage_Full, "Player.Status.Rage.Full", "玩家状态：愤怒满");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Rage_None, "Player.Status.Rage.None", "玩家状态：愤怒无");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_ComboWindow_Active_Light, "Player.Status.ComboWindow.Active.Light", "玩家状态：连招窗口激活-轻攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_ComboWindow_Active_Heavy, "Player.Status.ComboWindow.Active.Heavy", "玩家状态：连招窗口激活-重攻击");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Shipo, "Player.Status.Shipo", "玩家状态：识破");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_Charge_Looping, "Player.Status.Charge.Looping", "玩家状态：蓄力循环中");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Status_EnergyRegenCooldown, "Player.Status.EnergyRegenCooldown", "玩家状态：能量回复冷却中");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_ComboType_VerticalStaff, "Player.ComboType.VerticalStaff", "玩家连招类型：立棍");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_ComboType_HorizontalStaff, "Player.ComboType.HorizontalStaff", "玩家连招类型：横棍");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_ComboType_CleavingStaff, "Player.ComboType.CleavingStaff", "玩家连招类型：劈棍");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_ComboType_ThrustingStaff, "Player.ComboType.ThrustingStaff", "玩家连招类型：戳棍");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_SetByCaller_AttackType_Light, "Player.SetByCaller.AttackType.Light", "玩家设置：攻击类型-轻");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_SetByCaller_AttackType_Heavy, "Player.SetByCaller.AttackType.Heavy", "玩家设置：攻击类型-重");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_SetByCaller_PreEnergy, "Player.SetByCaller.PreEnergy", "玩家设置：气力扣除前值");

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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_ShipoSuccess, "Shared.Event.ShipoSuccess", "共享事件：识破成功");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_Freeze, "Shared.Event.Freeze", "共享事件：定身");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Ability_HitReact, "Shared.Ability.HitReact", "共享技能：受击反应");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Ability_Death, "Shared.Ability.Death", "共享技能：死亡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Ability_FellDown, "Shared.Ability.FellDown", "共享技能：倒地");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Ability_Freeze, "Shared.Ability.Freeze", "共享技能：定身");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_SetByCaller_BaseDamage, "Shared.SetByCaller.BaseDamage", "共享：基础伤害");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_SetByCaller_HealPercentage, "Shared.SetByCaller.HealPercentage", "共享：治疗百分比");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_SetByCaller_RageRecoverPercentage, "Shared.SetByCaller.RageRecoverPercentage", "共享：愤怒恢复百分比");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_SetByCaller_BaseRageGain, "Shared.SetByCaller.BaseRageGain", "共享：基础愤怒恢复");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_SetByCaller_RageGainOrCost, "Shared.SetByCaller.RageGainOrCost", "共享：愤怒增益或消耗");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_Dead, "Shared.Status.Dead", "共享状态：死亡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_HitReact_Front, "Shared.Status.HitReact.Front", "共享状态：受击反应-前");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_HitReact_Left, "Shared.Status.HitReact.Left", "共享状态：受击反应-左");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_HitReact_Right, "Shared.Status.HitReact.Right", "共享状态：受击反应-右");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_HitReact_Back, "Shared.Status.HitReact.Back", "共享状态：受击反应-后");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_Invincibility, "Shared.Status.Invincibility", "共享状态：无敌状态");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_FellDown, "Shared.Status.FellDown", "共享状态：倒地");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_Hegemony, "Shared.Status.Hegemony", "共享状态：霸体");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_ShipoWindow, "Shared.Status.ShipoWindow", "共享状态：识破窗口");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Status_Freezing, "Shared.Status.Freezing", "共享状态：冻结中");

	// Gameplay Tags for GameData
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameData_Level_SurvivalGameModeMap, "GameData.Level.SurvivalGameModeMap", "游戏数据：关卡-生存模式地图");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameData_Level_MainMenuMap, "GameData.Level.MainMenuMap", "游戏数据：关卡-主菜单地图");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_WidgetStack_Modal, "Frontend.WidgetStack.Modal", "前端：模态窗口堆栈");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_WidgetStack_GameMenu, "Frontend.WidgetStack.GameMenu", "前端：游戏菜单窗口堆栈");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_WidgetStack_HUD, "Frontend.WidgetStack.HUD", "前端：HUD窗口堆栈");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_WidgetStack_Frontend, "Frontend.WidgetStack.Frontend", "前端：前端窗口堆栈");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_Frontend_PressAnyKeyScreen, "Frontend.Widget.Frontend.PressAnyKeyScreen", "前端：前端窗口-按任意键屏幕");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_Frontend_MainMenuScreen, "Frontend.Widget.Frontend.MainMenuScreen", "前端：前端窗口-主菜单屏幕");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_Frontend_OptionsScreen, "Frontend.Widget.Frontend.OptionsScreen", "前端：前端窗口-选项屏幕");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_Frontend_AboutScreen, "Frontend.Widget.Frontend.AboutScreen", "前端：前端窗口-关于屏幕");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_Frontend_StoryScreen, "Frontend.Widget.Frontend.StoryScreen", "前端：前端窗口-故事屏幕");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_Frontend_DifficultyScreen, "Frontend.Widget.Frontend.DifficultyScreen", "前端：前端窗口-难度屏幕");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_Frontend_SaveLoadScreen, "Frontend.Widget.Frontend.SaveLoadScreen", "前端：前端窗口-存档加载屏幕");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_Frontend_CreditScreen, "Frontend.Widget.Frontend.CreditScreen", "前端：前端窗口-制作人员屏幕");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_ModalScreen_QuitScreen, "Frontend.Widget.ModalScreen.QuitScreen", "前端：模态屏幕-退出游戏");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_ModalScreen_ReturnBackToMainMenu, "Frontend.Widget.ModalScreen.ReturnBackToMainMenu", "前端：模态屏幕-返回主菜单");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_ModalScreen_More, "Frontend.Widget.ModalScreen.More", "前端：模态屏幕-更多");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_ModalScreen_Normal, "Frontend.Widget.ModalScreen.Normal", "前端：模态屏幕-普通");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_ModalScreen_TimerConfirm, "Frontend.Widget.ModalScreen.TimerConfirm", "前端：模态屏幕-计时确认");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_ModalScreen_KeyRemapScreen, "Frontend.Widget.ModalScreen.KeyRemapScreen", "前端：模态屏幕-按键重映射");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_ModalScreen_PlayerDie, "Frontend.Widget.ModalScreen.PlayerDie", "前端：模态屏幕-玩家死亡");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_ModalScreen_PlayerWin, "Frontend.Widget.ModalScreen.PlayerWin", "前端：模态屏幕-玩家胜利");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_HUD_KeyMouseOverlay, "Frontend.Widget.HUD.KeyMouseOverlay", "前端：HUD窗口-按键鼠标覆盖");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_HUD_GamePadOverlay, "Frontend.Widget.HUD.GamePadOverlay", "前端：HUD窗口-游戏手柄覆盖");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Widget_HUD_PauseScreen, "Frontend.Widget.HUD.PauseScreen", "前端：HUD窗口-暂停屏幕");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frontend_Image_TestImage, "Frontend.Image.TestImage", "前端：测试图像");
	
}
