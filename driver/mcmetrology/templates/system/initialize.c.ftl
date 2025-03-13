
    /* Initialize Metrology Driver Instance */
    sysObj.drvMCMet = DRV_MCMETROLOGY_Initialize((SYS_MODULE_INIT *)&drvMCMetrologyInitData, RSTC_ResetCauseGet());
    