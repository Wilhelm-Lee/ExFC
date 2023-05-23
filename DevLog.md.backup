# Develop Log

\[19th May, 2023 15:29:23\] (William)  
Now working on:  
____ Considering adding file "src/catcher.h" & file "src/catcher.c"  
____ Reason:  
____ ____ Having an exception thrown and ending the thread directly is not  
____ ____ __ ideal or safe.  
____ ____ Now suppose we could catch the specified exception and redirect
the  
____ ____ __ flow to target function could make throwing an exception  
____ ____ __ much more sensible.  
Finished:  
____ Basic readings and writings for exceptions.  
More to be done:  
____ Now [Carnival](https://github.com/WilliamPascal/Carnival) is bound to be
used in [JCCL](https://github.com/JackCraftTeam/JackCraftCommodoreLauncher).  
____ Not only JCCL is based on terminology, Carnival is born for terminals,  
____ __ but also, during the whole process of these two projects, I've
discoverd  
____ __ that, C & C++ has the most flexibilities towards low-level
technologies  
____ __ crediting to GNU/GCC and GNU/G++. (of course, they were born for it)

<h4 align="right">-- William Pascal</h4>
________________________________________________________________________________

\[21th May, 2023 19:27:32\] (William)  
Now working on:  
____ Testing current existing functionalities.  
____ Problems:  
____ ____ Function "`exception_addexcep(char*, char*, int)`" returned
`DUPLICATED -> -300`  
____ ____ __ even inputted parameter was valid.  
____ ____ The core problem, I found, was the handler of returning mechanic.  
____ ____ Which allocates at file "`src/exception.c`", in function  
____ ____ __ "`_exception_quick_match_str(char*, char*, bool)`".  
____ ____ This function, however, was returning "`bool`".  
____ ____ It was done writting before I got chance to create "`excep_return_E`".  
____ ____ So, it's inevitably to be misunderstandable by its return code handlers.  
____ Solution:  
____ ____ Since the only problem(probably) it to change the logic on returning,  
____ ____ __ this problem could be solved.  
<h4 align="right">-- William Pascal</h4>
________________________________________________________________________________
