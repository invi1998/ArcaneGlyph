# 自定义绑定动作按钮（Custom Bound Action Buttons）

1. ## 定义键位映射

   指定触发动作按钮的按键。关于键位映射，我们可以查看现有的默认输入映射资产（`CommonUIInputData`），可以看到，对于默认的绑定动作，`CommonUI`为这两个变量填充了两个不同的行，一个是确认操作，一个是返回操作，这些操作数据行来自我们的通用输入键位映射数据表。

   ![image-20250609192556267](.\image-20250609192556267.png)

   然后这个数据资产在我们的通用UI的项目设置里被指定进而被使用。

   ![image-20250609192832768](.\image-20250609192832768.png)

   然后我们在源码里查看该类型资产的源码`UCommonUIInputData`，可以看到有两个数据表行句柄类型的变量，被标记为了仅限默认值，而且他们都使用了相同的方法限定符来限制我们能为该变量填充的数据表数量`meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase")`，而这一设置也是我们可以在自定义绑定动作里可以使用的设置。

   ```c++
   /* Derive from this class to store the Input data. It is referenced in the Common Input Settings, found in the project settings UI. */
   UCLASS(Abstract, Blueprintable, ClassGroup = Input, meta = (Category = "Common Input"))
   class COMMONINPUT_API UCommonUIInputData : public UObject
   {
   	GENERATED_BODY()
   
   public:
   	virtual bool NeedsLoadForServer() const override;
   
   public:
   	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
   	FDataTableRowHandle DefaultClickAction;
   
   	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
   	FDataTableRowHandle DefaultBackAction;
   
   	/**
       * Newly created CommonButton widgets will use these hold values by default if bRequiresHold is true.
       * Inherits from UCommonUIHoldData.
       */
       UPROPERTY(EditDefaultsOnly, Category = "Properties")
       TSoftClassPtr<UCommonUIHoldData> DefaultHoldData;
   
   	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (EditCondition = "CommonInput.CommonInputSettings.IsEnhancedInputSupportEnabled", EditConditionHides))
   	TObjectPtr<UInputAction> EnhancedInputClickAction;
   
   	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (EditCondition = "CommonInput.CommonInputSettings.IsEnhancedInputSupportEnabled", EditConditionHides))
   	TObjectPtr<UInputAction> EnhancedInputBackAction;
   };
   ```

   了解这一点后，我们来看看如何在选项界面注册自定义按钮。

   首先，我想在Option页面添加一个重设按钮，将选项设置恢复到默认值，所以我们在Widget中添加一个重设的操作的数据行句柄

   ```c++
   #include "CoreMinimal.h"
   #include "Widget/Widget_ActivatableBase.h"
   #include "Widget_OptionsBase.generated.h"
   
   /**
    * 
    */
   UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
   class ARCANEGLYPH_API UWidget_OptionsBase : public UWidget_ActivatableBase
   {
   	GENERATED_BODY()
   
   protected:
   	
   	// begin UWidget interface
   	virtual void NativeOnInitialized() override;
   	// end UWidget interface
   
   private:
   	// 重设操作的行数据句柄
   	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
   	FDataTableRowHandle ResetActions;
   
   	FUIActionBindingHandle ResetBindingHandle;	// 重设操作的绑定句柄
   
   	// 重设操作按钮点击事件 （由于我们不是绑定到动态多播委托，只是简单的委托绑定，所以该绑定函数不需要UFUNCTION）
   	void OnResetBoundActionsTriggered();
   	void OnBackBoundActionsTriggered();
   };
   
   ```

   

   然后我们向绑定操作栏插入自定义绑定操作并保存绑定句柄（`RegisterUIActionBinding`），添加玩完成自定义绑定后，我们可以注册默认的返回操作，以便我们的Option页面也可以正常返回到上一个路由页面

   ```c++
   void UWidget_OptionsBase::NativeOnInitialized()
   {
   	Super::NativeOnInitialized();
   
   	if (!ResetActions.IsNull())
   	{
   		// 向绑定操作栏插入自定义绑定操作并保存绑定句柄
   		ResetBindingHandle = RegisterUIActionBinding(
   			FBindUIActionArgs(
   				ResetActions,
   				true, // 在操作栏中显示
   				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionsTriggered)
   			)
   		);
   	}
   
   	// 完成自定义绑定后，我们可以注册默认的返回操作
   	RegisterUIActionBinding(
   		FBindUIActionArgs(
   			ICommonInputModule::GetSettings().GetDefaultBackAction(),
   			true, // 在操作栏中显示
   			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionsTriggered)
   		)
   	);
   }
   
   void UWidget_OptionsBase::OnResetBoundActionsTriggered()
   {
   	Debug::Print(TEXT("Reset Bound Actions Triggered!"), FColor::Green);
   }
   
   void UWidget_OptionsBase::OnBackBoundActionsTriggered()
   {
   	// 返回操作触发时，通常会关闭当前选项界面
   	DeactivateWidget();
   }
   ```

   

   

2. ## 调用 `Register UI Action Binding` 完成绑定