# ProgrammingLanguage

You would need to run this command on terminal  'chmod +x sea'

1. All statements will need a semicolon in the end.

2. Comments- In line comments can be created by `
    `def x = "This is a comment";
    def x = "This is not a comment";

3. List of all keywords (Keywords are not case sensitive): 

    def - Use to declare variables
        def var1 = "Hello";
        def var2 = 0;
        def var3 = var1;
        def funcName = lambda ( arg1, arg2)
        {
            body;
        }; (Function)

    set - sets a variable;
        set var1 = var2;
        set var1 = call funcName( arg);

   
    
    if/else - Conditionals
    The else is optional 
       
        if (condition)
        {
            body executed if condition is true
        } 
        else (Optional Else )
        {
            body executed if condition is False
        }; 

     Comparisons: <, >, equals
      if ( 3 < 4 )
    {
        body executed - condition is true
    } 
    else 
    {
        body not executed  - condition is true
    };

        if ( 3 equals 4 )
    {
        body NOT executed - condition is False
    } 
    else 
    {
        body executed  - condition is False
    };
    

    Chaining conditions-  Or, And :

       def x = "hello";
        if ( 3 < 4  and 3 equals 3 or "hello" equals x )
        {
            body executed - condition is true
        } 
        else 
        {
            body not executed  - ondition is False
        }; 

   



    Math Expression (  + - * / % ) - 

    Math expressions are enlcosed by parens and are right associative.
    The set of mathematical oparands are + - * / %
    
    def x =  (2 + 3 + (4 / 2 ) );


    lambda - use to define a function (Named Or Annonymous)
    Functions are first class objects.
        def funcName = lambda ( arg1, arg2)
        {body}; (Function)

        lambda (arg1, arg2)
        {
            body;
        }; (Annonymous Function)

    call - calls a function
        def funcName = lambda ( arg1, arg2); (Function)
        set var1 = call funcName( arg1, "Argument 2");
        display call funcName( arg1, "Argument 2");

    While - Iteration 

	while ( i < j )
	{
	display "Element ";
	display i;
	display "\n";
	set i = (i + 1);
	};

    Arrays:
    defArray - defines an array, takes size and optional elements
    SetArray - sets the array's elememnt at the given index
    callArray- gets the array's elememnt at the given index
    

    defarray x[size] = { 1, 2, 3, var1, 5};
    def i =0;

    while( i < size)
    {
    display callArray x[i];
     set i = (i + 1);
    };
    index = 3;
    setArray[index] = "HELLO";