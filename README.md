This is a basic interpreter written for Monkey-lang, which appears in this [book](https://interpreterbook.com/). Though it was written in Go, it was easy to move it to C++.

I was able to go get the following working:
- Expression evaluation
- Conditional statements
- Variable definitions
- Functions 

But there are still some bugs I need to fix
```bash
>>let a = 5; 
5
>>let b = fn(x,y) { x*y};; 
>>b(a,6);
30
```