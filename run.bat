set "funcNames=Ackley Rastrigin HappyCat Rosenbrock Zakharov Michalewicz"

for %%i in (%funcNames%) do (
    echo Running main.exe with funcName: %%i
    main.exe 10 %%i
)