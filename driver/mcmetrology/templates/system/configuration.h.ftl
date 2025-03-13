
/* Metrology Configuration Options */
#define DRV_MCMETROLOGY_REG_BASE_ADDRESS            0x${DRV_MCMET_BASE_ADDRESS?string}UL

/* Metrology library Binary file addressing */
extern uint8_t met_bin_start;
extern uint8_t met_bin_end;

<#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS != "BareMetal">
    <#lt>/* Metrology Driver RTOS Configurations */
    <#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "FreeRTOS">
        <#lt>#define DRV_MCMETROLOGY_RTOS_STACK_SIZE         ${DRV_MCMET_RTOS_STACK_SIZE / 4}U
    <#else>
        <#lt>#define DRV_MCMETROLOGY_RTOS_STACK_SIZE         ${DRV_MCMET_RTOS_STACK_SIZE}U
    </#if>
        <#lt>#define DRV_MCMETROLOGY_RTOS_TASK_PRIORITY      ${DRV_MCMET_RTOS_TASK_PRIORITY}U
        <#if (HarmonyCore.SELECT_RTOS)?? && HarmonyCore.SELECT_RTOS == "MicriumOSIII">
            <#lt>#define DRV_MCMETROLOGY_RTOS_TASK_MSG_QTY       ${DRV_MCMET_RTOS_TASK_MSG_QTY}U
            <#lt>#define DRV_MCMETROLOGY_RTOS_TASK_TIME_QUANTA   ${DRV_MCMET_RTOS_TASK_TIME_QUANTA}U
    </#if>
</#if>