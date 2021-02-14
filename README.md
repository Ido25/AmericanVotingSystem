
# Object oriented programming in CPP
=======================================================

## Round Save To Bin File Format:
### All String Saved Without \0

| `TYPE`              | `NAME`             | `SIZE IN BYTES`  |
| ------------------- | ------------------ | ---------------  |
| unsigned char       | round type         | 1                |
| `ROUND DATA`                                                |
| unsigned char       | year               | 1                |
| unsigned char       | month              | 1                |
| unsigned char       | day                | 1                |
| short int           | prt log size       | 2                |
| short int           | prt phy size       | 2                |
| short int           | dst log size       | 2                |
| short int           | dst phy size       | 2                |
| short int           | already votes size | 2                |
| char                | id                 | id amount        |
|                                                             |
| `DISTRICTS DATA`                                            |
| byte                | unitedDistrict     | 1                |
| short int           | id                 | 2                |
| unsigned char       | name length        | 1                |
| char                | name               | name length      |
| short int           | amount of reps     | 2                |
| float               | vote rate          | 4                |
|                                                             |
| `CITIZEN MAP DATA`                                          |
| short int           | length             | 2                |
| `CITIZEN DATA`                                              |
| unsigned char       | name length        | 1                |
| char                | name               | name length      |
| unsigned char       | id length          | 1                |
| char                | id                 | id length        |
| short int           | birth year         | 2                |
| bool                | vote status        | 1                |
|                                                             |
| `PARTIES DATA`                                              |
| short int           | id                 | 2                |
| unsigned char       | name length        | 1                |
| char                | name               | name length      |
| Citizen             | leader             | Citizen size     |
| short int           | members arr length | 2                |
|                                                             |
| short int           | length per dst     | 2                |
| Citizen             | elector            | Citizen size     |
|                                                             |
| `FOR REGULAR`                                               |
| short int           | votes row size     | 2                |
| short int           | votes col size     | 2                |
| `FOR SIMPLE`                                                |
| short int           | votes size         | 2                |
| `VOTES ARRAY FOR BOTH`                                      |
| unsigned int        | amount             | 4                |
| unsigned char       | name length        | 1                |
| char                | party name         | name length      |

#### char [-128,127]
#### short int [-32768, 32767]
#### int [-2147483648,2147483647]

## Changes for Assignment 3
#1. Update CitizenTree structure to Dictionary (STL::map()) - DONE
##2. Change realloc function to a template function
##5. Build DynamicArray class and implement dst_array and prt_array with class (including iterators)
##8. Check usage of object function and operator() in getting round function to write into files
#9. Transforming matching functions to Lambda functions
#14. Fixing writing to file after changing everything - need to add set writing and reading
#15. change quicksort in round to sort algorithm from <algorithm>
#16. fix assignment 2 rejects