# Proto7
A simple stack-based bytecode interpreter written in C.


# TODOs
- Error if calling label with no ret keyword


## Instruction set
| Op | Effect | Description |
| - | - | - |
| Push <n> | -- a      | Push the literal value `n` onto the stack. | 
| | |
| Add | a b -- (b+a)   | Pop the top two values and push their sum. |
| Sub | a b -- (b-a)   | Pop the top two values and push their difference. |
| Mul | a b -- (b*a)   | Pop the top two values and push their product.  |
| Div | a b -- (b/a)   | Pop the top two values and push their quotient. |
| | | 
| And | a b -- (b&a)| Pop the top two values and push their bitwise AND. |
| Or | a b -- (b|a) | Pop the top two values and push their bitwise OR.  |
| Xor | a b -- (b^a)| Pop the top two values and push their bitwise XOR. |
| Not | a -- (~a)   | Pop the top value and push its bitwise complement. |
| Shl | a b -- (b << a)| Pop the top two values and push the result of shifting b left by a bits. |
| Shr | a b -- (b >> a)| Pop the top two values and push the result of shifting b right by a bits.|
| | |
| Eq  | a b -- (b == a)| Push `1` if the values are equal, otherwise `0`. |
| Gt  | a b -- (b > a) | Push `1` if b is greater than a, otherwise `0`.  | 
| Lt  | a b -- (b < a) | Push `1` if b is less than a, otherwise `0`.     |
| | |
| Dup | a -- a a       | Duplicate the top value on the stack. |
| Over| a b -- a b a   | Copy the second value to the top of the stack. |
| Swap| a b -- b a     | Swap the top two values. |
| Rot | a b c -- b c a | Rotate the top three values. |
| Drop| a b -- a       | Remove the top value from the stack. |
| | |
| Alloc <n>/stack | -- ptr | Allocate `n` bytes of memory and push the resulting `ptr`. |
| Write <n> | ptr value -- | Store value as an `n`-byte value at `ptr`. |
| Read  <n> | ptr -- value | Read an `n`-byte value from `ptr` and push it onto the stack. |
| Free | ptr -- | Free the memory pointed to by `ptr`. |
| | |
| <label > | | Defines a jump/call target. |
| JMP <label> | | Unconditionally jump to `label`. |
| JT <label> | a -- | Jump to `label` if cond is `1`. |
| JF <label> | a -- | Jump to `label` if cond is `0`. |
| call <label> | | Call a subroutine at `label`. |
| ret | | Return from the current subroutine. |
| dump | a -- | Print the top value of the stack |
| syscall <n> | nr \[..n\] -- | Invoke system call with `n` number of arguments, consuming the required arguments and syscall `number` from the stack. |
