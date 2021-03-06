Memory Allocator - Project  
  
References: https://ocw.cs.pub.ro/courses/programare/teme_2017/proiect_2017_ca  
  
Available commands:  
  
INITIALIZE n  
FINALIZE  
DUMP  
ALLOC size  
FREE index  
FILL index size value  
SHOW info  

info can be one of the following: FREE, USAGE, ALLOCATIONS    

The programs runs until it receives the FINALIZE command.    

EXAMPLE 1:  

INITIALIZE 100  
ALLOC 13  
16  
FILL 16 13 255  
DUMP  
00000000	04 00 00 00 00 00 00 00 00 00 00 00 19 00 00 00  
00000010	FF FF FF FF FF FF FF FF FF FF FF FF FF 00 00 00  
00000020	00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  
00000030	00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  
00000040	00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  
00000050	00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  
00000060	00 00 00 00  
00000064  
FREE 16  
ALLOC 50  
16  
ALLOC 40  
0  
ALLOC 30  
0  
ALLOC 20  
78  
FILL 78 20 127  
DUMP  
00000000	04 00 00 00 42 00 00 00 00 00 00 00 3E 00 00 00   
00000010	FF FF FF FF FF FF FF FF FF FF FF FF FF 00 00 00   
00000020	00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
00000030	00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   
00000040	00 00 00 00 00 00 04 00 00 00 20 00 00 00 7F 7F  
00000050	7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F   
00000060	7F 7F 00 00   
00000064  
FREE 16  
FREE 78  
FINALIZE  
  
EXAMPLE 2:  
  
INITIALIZE 100  
ALLOC 10  
16  
ALLOC 10  
38  
ALLOC 10  
60  
ALLOC 10  
82  
ALLOC 10  
0  
FREE 16  
FREE 60  
FILL 38 10 255  
FILL 82 10 255  
DUMP  
00000000	1A 00 00 00 1A 00 00 00 00 00 00 00 16 00 00 00   
00000010	00 00 00 00 00 00 00 00 00 00 46 00 00 00 00 00  
00000020	00 00 16 00 00 00 FF FF FF FF FF FF FF FF FF FF  
00000030	46 00 00 00 1A 00 00 00 16 00 00 00 00 00 00 00   
00000040	00 00 00 00 00 00 00 00 00 00 1A 00 00 00 16 00  
00000050	00 00 FF FF FF FF FF FF FF FF FF FF 00 00 00 00   
00000060	00 00 00 00   
00000064  
FINALIZE  
  
EXAMPLE 3:  
  
INITIALIZE 100  
ALLOC 20  
16  
ALLOC 20  
48  
SHOW FREE  
1 blocks (32 bytes) free  
SHOW USAGE  
2 blocks (40 bytes) used  
58% efficiency  
0% fragmentation  
SHOW ALLOCATIONS  
OCCUPIED 4 bytes  
OCCUPIED 32 bytes  
OCCUPIED 32 bytes  
FREE 32 bytes  
FREE 16  
SHOW FREE  
2 blocks (64 bytes) free  
SHOW USAGE  
1 blocks (20 bytes) used  
55% efficiency  
100% fragmentation  
SHOW ALLOCATIONS  
OCCUPIED 4 bytes  
FREE 32 bytes  
OCCUPIED 32 bytes  
FREE 32 bytes  
FINALIZE  
  
  
Antonio Dan Macovei  


