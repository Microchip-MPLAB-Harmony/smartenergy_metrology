
<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "ThreadX">
    <#lt>TX_THREAD      lDRV_MCMETROLOGY_Task_TCB;
    <#lt>uint8_t*       lDRV_MCMETROLOGY_Task_Stk_Ptr;

<#elseif (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "MicriumOSIII">
    <#lt>OS_TCB  lDRV_MCMETROLOGY_Tasks_TCB;
    <#lt>CPU_STK lDRV_MCMETROLOGY_TasksStk[DRV_MCMETROLOGY_RTOS_STACK_SIZE];

</#if>
    <#lt>static void lDRV_MCMETROLOGY_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(true)
    <#lt>    {
    <#lt>        DRV_MCMETROLOGY_Tasks(sysObj.drvMCMet);
    <#lt>    }
    <#lt>}
