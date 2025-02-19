# ArcaneGlyph

![image-20250219131416782](Note\image-20250219131416782.png)

在开发过程中，使用`ShowDebug AbilitySystem`来调试GE的时候，如果遇到使用`pageUp`或者`pageDown`切换查看对象的`AttributeSets`属性集的时候数据不正确（不刷新），可以修改项目的游戏配置项`DefaultGame.ini`，添加如下配置开启debug

```ini
[/Script/GameplayAbilities.AbilitySystemGlobals]
bUseDebugTargetFromHud = true
```

