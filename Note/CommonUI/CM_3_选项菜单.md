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

   

   

2. ## 调用 `Register UI Action Binding` 完成绑定