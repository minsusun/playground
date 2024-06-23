### DTO
- Data Transfer Object, the main purpose of the pattern is to reduce the number of methods calls(reduce roundtrips to the server by batching up multiple parameters in a single call => reduce network overhead)
- Additionally, we can hide informations instead of exposing all the fields

### C# null conditional operator(?.)
- it can be interpreted as null-conditional member operator
- if left-hand operand is null, return null
- else, return the right-hand member

### C# bit-wise shift operators
- `>>>`: logical shift
- `>>`: arithmetic shift
- `<<`: discards high-order bits, fill left with zeros

### C# null co-alescing operator(??)
- return left-hand operand when left-hand operand is not null
- return right-hand operand when left-hand operand is null

### C# pattern matching in switch-case statement
- pack the data into object datatype
- then, we can use datatype to specify each cases
- (additional) we can also use **case guard** within thet each cases
- so, we can write like this: `case float f when f >= 0:`
- this corresponds to the statement that given argument is float and bigger than 0

### C# switch expression
- we can use switch expression in C#
- i.e. 
    ```C#
    string grade = score switch
    {
        90 when repeated == true => "B+",
        90 => "A",
        80 => "B",
        70 => "C",
        60 => "D",
        _ => "F"        // <- discard pattern in C#
    }
    ```

