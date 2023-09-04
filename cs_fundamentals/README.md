# Google Prep Course (python version)

## 1.) watch these 2 videos
* https://www.youtube.com/watch?v=qc1owf2-220&feature=youtu.be
* https://www.youtube.com/watch?v=oWbUtlUhwa8&feature=youtu.be

## 2.) Review your past jobs and side projects
* Think of all the things you did that were instering, then think of the most  most interesting solutions you came up to sovle the issue at hand
* google says this: 
Your background is important, so utilize it to your advantage by sharing interesting points that can help you and the engineer work through the discussion as efficient as possi

__## Computer Science fundementals__

## 3.) Algorithum Complexity 
* big-o notaion (must know or you will fail)
big-o-notation =  A theoretical measure of the execution of an algorithm, usually the time or memory needed, given the problem size n, which is usually the number of items. Informally, saying some equation f(n) = O(g(n)) means it is less than some constant multiple of g(n).

![Table](/../master/images/bigo.png?raw=true "Algorithum table")
![Graph](/../master/images/bigograph.png??raw=true "Graph")

* https://www.topcoder.com/blog/big-o-notation-primer/  

O(1) = O(1) represents an algorithm that takes the same amount of time to execute regardless of the number of inputs. So, 1 item takes 1 second, 10 items take 1 second, 100 items take 1 second and so on. Therefore performance is not affected by the size of the input data.

O(N) = O(N) represents an algorithm where the size of the input data impacts the execution time. The performance of the algorithm is directly proportional to the number of inputs. So, 1 item takes 1 second, 10 items take 10 seconds, 100 items take 100 seconds and so on.

O(log N) = O(log N) represents an algorithm where the number of computations grows linearly as input data grows exponentially. So 1 item takes 1 second, 10 items take 2 seconds, 100 items take 3 seconds and so on.

O(2N) = O(2N) represents an algorithm where execution time is doubled for each additional input. So 1 item takes 2 seconds, 2 items take 4 seconds, 3 items take 8 seconds and so on.

O(N!) = O(N!) represents a factorial algorithm that must perform N! calculations. So 1 item takes 1 second, 2 items take 2 seconds, 3 items take 6 seconds and so on. An example of a this algorithm is one that recursively calculates fibonacci numbers.

O(N^2) = O(N^2) represents an algorithm that is directly proportional to the square of the sizes of the inputs and must performs N^2 calculations (by definition). Bubblesort is a good example of this algorithm.

O(N log N) = represents an algorithm that will in increase in execution time proportionate to the number of the input times the logarithm of the number of the input. Mergesort and quicksort are good examples of this algorithm.

![Big-O Complexity Chart](/../master/images/bigocomplexitychart.png?raw=true "Big-O Complexity Chart")
![Common Data Structure Operations](/../master/images/commonDataStructureOperations.png??raw=true "Common Data Structure Operations")
![Array Sorting Algorithms](/../master/images/arraySortingAlgos.png?raw=true "Array Sorting Algorithms")
![Big-O-Cheatsheet](/../master/images/big-o-cheat-sheet-poster.png?raw=true "Big-o-cheatsheet")

The Telephone Book

The next best example I can think of is the telephone book, normally called the White Pages or similar but it'll vary from country to country. But I'm talking about the one that lists people by surname and then initials or first name, possibly address and then telephone numbers.

Now if you were instructing a computer to look up the phone number for "John Smith" in a telephone book that contains 1,000,000 names, what would you do? Ignoring the fact that you could guess how far in the S's started (let's assume you can't), what would you do?

A typical implementation might be to open up to the middle, take the 500,000th and compare it to "Smith". If it happens to be "Smith, John", we just got real lucky. Far more likely is that "John Smith" will be before or after that name. If it's after we then divide the last half of the phone book in half and repeat. If it's before then we divide the first half of the phone book in half and repeat. And so on.

This is called a binary search and is used every day in programming whether you realize it or not.

So if you want to find a name in a phone book of a million names you can actually find any name by doing this at most 20 times. In comparing search algorithms we decide that this comparison is our 'n'.

For a phone book of 3 names it takes 2 comparisons (at most).
For 7 it takes at most 3.
For 15 it takes 4.
…
For 1,000,000 it takes 20.
That is staggeringly good isn't it?

In Big-O terms this is O(log n) or logarithmic complexity. Now the logarithm in question could be ln (base e), log10, log2 or some other base. It doesn't matter it's still O(log n) just like O(2n2) and O(100n2) are still both O(n2).

It's worthwhile at this point to explain that Big O can be used to determine three cases with an algorithm:

Best Case: In the telephone book search, the best case is that we find the name in one comparison. This is O(1) or constant complexity;
Expected Case: As discussed above this is O(log n); and
Worst Case: This is also O(log n).
Normally we don't care about the best case. We're interested in the expected and worst case. Sometimes one or the other of these will be more important.

Back to the telephone book.

What if you have a phone number and want to find a name? The police have a reverse phone book but such look-ups are denied to the general public. Or are they? Technically you can reverse look-up a number in an ordinary phone book. How?

You start at the first name and compare the number. If it's a match, great, if not, you move on to the next. You have to do it this way because the phone book is unordered (by phone number anyway).

So to find a name given the phone number (reverse lookup):

Best Case: O(1);
Expected Case: O(n) (for 500,000); and
Worst Case: O(n) (for 1,000,000).
The Travelling Salesman

This is quite a famous problem in computer science and deserves a mention. In this problem you have N towns. Each of those towns is linked to 1 or more other towns by a road of a certain distance. The Travelling Salesman problem is to find the shortest tour that visits every town.

Sounds simple? Think again.

If you have 3 towns A, B and C with roads between all pairs then you could go:

A → B → C
A → C → B
B → C → A
B → A → C
C → A → B
C → B → A
Well actually there's less than that because some of these are equivalent (A → B → C and C → B → A are equivalent, for example, because they use the same roads, just in reverse).

In actuality there are 3 possibilities.

Take this to 4 towns and you have (iirc) 12 possibilities.
With 5 it's 60.
6 becomes 360.
This is a function of a mathematical operation called a factorial. Basically:

5! = 5 × 4 × 3 × 2 × 1 = 120
6! = 6 × 5 × 4 × 3 × 2 × 1 = 720
7! = 7 × 6 × 5 × 4 × 3 × 2 × 1 = 5040
…
25! = 25 × 24 × … × 2 × 1 = 15,511,210,043,330,985,984,000,000
…
50! = 50 × 49 × … × 2 × 1 = 3.04140932 × 1064
So the Big-O of the Travelling Salesman problem is O(n!) or factorial or combinatorial complexity.

By the time you get to 200 towns there isn't enough time left in the universe to solve the problem with traditional computers.

Something to think about.

Polynomial Time

Another point I wanted to make quick mention of is that any algorithm that has a complexity of O(na) is said to have polynomial complexity or is solvable in polynomial time.

O(n), O(n2) etc are all polynomial time. Some problems cannot be solved in polynomial time. Certain things are used in the world because of this. Public Key Cryptography is a prime example. It is computationally hard to find two prime factors of a very large number. If it wasn't, we couldn't use the public key systems we use.

Law of Addition for big-O notation

 O(f(n)) + O(g(n)) is O( f(n) + g(n) )

That is, we when adding complexity classes we bring the two complexity classes
inside the O(...). Ultimately, O( f(n) + g(n) ) results in the bigger of the two
complexity class (because we drop the lower added term). So,

O(N) + O(Log N)  =  O(N + Log N)  =  O(N)

because N is the faster growing function.

This rule helps us understand how to compute the complexity of doing some 
SEQUENCE of operations: executing a statement that is O(f(n)) followed by
executing a statement that is O(g(n)). Executing both statements SEQUENTAILLY
is O(f(n)) + O(g(n)) which is O( f(n) + g(n) ) by the rule above.

For example, if some function call f(...) is O(N) and another function call
g(...) is O(N Log N), then doing the sequence

   f(...)
   g(...)

is O(N) + O(N Log N) = O(N + N Log N) = O(N Log N). Of course, executing the
sequence (calling f twice)

  f(...)
  f(...)

is O(N) + O(N) which is O(N + N) which is O(2N) which is O(N).

Note that for an if statment like

  if test:    	 assume complexity of test is O(T)
     block 1     assume complexity of block 1 is O(B1)
  else:
     block 2     assume complexity of block 2 is O(B2)

The complexity class for the if is O(T) + max(O(B1),O(B2)). The test is always
evaluated, and one of the blocks is always executed. In the worst case, the if
will execute the block with the largest complexity. So, given

  if test:    	 complexity is O(N)
     block 1     complexity is O(N**2)
  else:
     block 2     complexity is O(N)

The complexity class for the if is O(N) + max (O(N**2),O(N))) = O(N) + O(N**2) =
O(N + N**2) = O(N**2). If the test had complexity class O(N**3), then the
complexity class for the if is O(N**3) + max (O(N**2),O(N))) = 
O(N**3) + O(N**2) = O(N**3 + N**2) = O(N**3).

------------------------------------------------------------------------------

Law of Multiplcation for big-O notation

 O(f(n)) * O(g(n)) is O( f(n) * g(n) )

If we repeat an O(f(N)) process O(N) times, the resulting complexity is
O(N)*O(f(N)) = O( Nf(N) ). An example of this is, if some function call f(...)
is O(N**2), then executing that call N times (in the following loop)

  for i in range(N):
    f(...)

is O(N)*O(N**2) = O(N*N**2) = O(N**3)

This rule helps us understand how to compute the complexity of doing some 
statement INSIDE A BLOCK controlled by a statement that is REPEATING it. We
multiply the complexity class of the number of repetitions by the complexity
class of the statement(s) being repeated.

Compound statements can be analyzed by composing the complexity classes of
their constituent statements. For sequential statements the complexity classes
are added; for statements repeated in a loop the complexity classes are
multiplied.

Let's use the data and tools discussed above to analyze (determine their
complexity classes) three different functions that each compute the same
result: whether or not a list contains only unique values (no duplicates). We
will assume in all three examples that len(alist) is N.

1) Algorithm 1: A list is unique if each value in the list does not occur in any
later indexes: alist[i+1:] is a list containing all values after the one at
index i.

def is_unique1 (alist : [int]) -> bool:
    for i in range(len(alist)):		O(N)
        if alist[i] in alist[i+1:]:	O(N) - index+add+slice+in: O(1)+O(1)+O(N)+O(N) = O(N)
            return False		O(1) - never executed in worst case
    return True				O(1)

The complexity class for executing the entire function is O(N) * O(N) + O(1)
= O(N**2). So we know from the previous lecture that if we double the length of
alist, this function takes 4 times as long to execute.

Note that in the worst case, we never return False and keep executing the loop,
so this O(1) does not appear in the answer. Also, in the worst case the list
slice is aliset[1:] which is O(N-1) = O(N).

2) Algorithm 2: A list is unique if when we sort its values, no ADJACENT values
are equal. If there were duplicate values, sorting the list would put these
duplicate values right next to each other (adjacent). Here we copy the list so
as to not mutate (change the order of the parameter's list) by sorting it:
it turns out that copying the list does not increase the complexity class of
the method.

def is_unique2 (alist : [int]) -> bool:
    copy = list(alist)			O(N)
    copy.sort()				O(N Log N) - for fast Python sorting
    for i in range(len(alist)-1):	O(N) - really N-1, but that is O(N)
        if copy[i] == copy[i+1]:	O(1): +, 2 [i],and  == ints: all O(1)
            return False		O(1) - never executed in worst case
    return True	   			O(1)

The complexity class for executing the entire function is given by the sum
O(N) + O(N Log N) + O(N)*O(1) + O(1) = O(N + N Log N + O(N*1) + 1) =
O(N + N Log N + N + 1) = O(N Log N + 2N + 1) = O(N Log N). So the
complexity class for this algorithm/function is lower than the first algorithm,
the is_unique1 function. For large N unque2 will eventually be faster.

Notice that the complexity class for sorting is dominant in this code: it does
most of the work. If we double the length of alist, this function takes a bit
more than twice the amount of time. In N Log N: N doubles and Log N gets a tiny
bit bigger (i.e., Log 2N = 1 + Log N; e.g., Log 2000 = 1 + Log 1000 = 11, so
compared to 1000 Log 1000, 2000 Log 2000 got 2.2 times bigger, or 10% bigger
than just doubling).

Looked at another way if T(N) = c*(N Log N), then T(2N) = c*(2N Log 2N) =
c*2N Log N + c*2N = 2*T(N) + c*2N. Or, computing the doubling signature

T(2N)    c*2(N Log N) + c*2N            2
----- =  -------------------  =  2 + -------
T(N)          c*(N Log N)             Log N

So, the ratio is 2 + a bit (and that bit gets smaller as N increases)

3) Algorithm 3: A list is unique if when we turn it into a set, its length is
unchanged: if duplicate values were added to the set, its length would be
smaller than the length of the list by exactly the number of duplicates in the
list added to the set.

def is_unique3 (alist : [int]) -> bool:
    aset = set(alist)			O(N): construct set from alist values
    return len(aset) == len(alist)	O(1): 2 len (each O(1)) and == ints O(1)

The complexity class for executing the entire function is O(N) + O(1) =
O(N + 1) = O(N). So the complexity class for this algortihm/function is lower
than both the first and second algorithms/functions. If we double the length of
alist, this function takes just twice the amount of time. We could write the
body of this function more simply as: return len(set(alist)) == len(alist),
where evaluating set(alist) takes O(N) and then computing the two len's and
comparing them for equality are all O(1).

So the bottom line here is that there might be many algorithms/functions to
solve some problem. If they are small, we can analyze them statically (looking
at the code, not running it) to determine their complexity classes. For large
problem sizes, the algorithm/function with the smallest complexity class will
be best. For small problem sizes, complexity classes don't determine which is
best (we need to account for the constants and lower order terms when sizes are
small), but we could run the functions (dynamic analysis, aka emperical
analysis) to test which is fastest on small size.

* https://www.topcoder.com/community/data-science/data-science-tutorials/computational-complexity-section-1/



## 4.) Coding (pick you language, C++ idealy im going to pick python)

###read the following
* Programming Interviews Exposed; Secrets to landing your next job by John Monagan and Noah Suojanen (Wiley Computer Publishing)
http://www.wiley.com/WileyCDA/WileyTitle/productCd-047012167X.html

## 5. systems
* system design:  http://research.google.com/pubs/DistributedSystemsandParallelComputing.html
like 182 publications read some

great intro to distibuted system design - Google code University
http://www.hpcs.cs.tsukuba.ac.jp/~tatebe/lecture/h23/dsys/dsd-tutorial.html

great into to parallel computing
https://computing.llnl.gov/tutorials/parallel_comp/

* google file system: http://research.google.com/archive/gfs.html
![Google File system](/../master/pdfs/googleFileSystem.pdf?raw=true "google file system")

* google bigtable:  http://research.google.com/archive/bigtable.html
![Google big table](/../master/pdfs/googleBigTable.pdf?raw=true "google big table")

* Google MapReduce: http://research.google.com/archive/mapreduce.html
![Google mapreduce](/../master/pdfs/googlemapreduce.pdf?raw=true "google mapreduce")

## 6. Sorting
* know how to sort, bubble-sort least important but know it

#### quicksort:
Quicksort is a very efficient sorting algorithm invented by C.A.R. Hoare. It has two phases:

the partition phase
the sort phase
Most of the work is done in the partition phase - it works out where to divide the work. The sort phase simply sorts the two smaller problems that are generated in the partition phase.

![Quick Sort](/../master/images/quicksort.png?raw=true "Quick sort ex")

This makes Quicksort a good example of the divide and conquer strategy for solving problems. This is similar in principle to the binary search. In the quicksort, we divide the array of items to be sorted into two partitions and then call the quicksort procedure recursively to sort the two partitions, ie we divide the problem into two smaller ones and conquer by solving the smaller ones.

http://www.csanimated.com/animation.php?t=Quicksort

```python
  def quicksort(myList, start, end):
    if start < end:
        # partition the list
        pivot = partition(myList, start, end)
        # sort both halves
        quicksort(myList, start, pivot-1)
        quicksort(myList, pivot+1, end)
    return myList

  def partition(myList, start, end):
    pivot = myList[start]
    left = start+1
    right = end
    done = False
    while not done:
        while left <= right and myList[left] <= pivot:
            left = left + 1
        while myList[right] >= pivot and right >=left:
            right = right -1
        if right < left:
            done= True
        else:
            # swap places
            temp=myList[left]
            myList[left]=myList[right]
            myList[right]=temp
    # swap start with myList[right]
    temp=myList[start]
    myList[start]=myList[right]
    myList[right]=temp
    return right
```

#### mergesort:
Merge sort is a sorting technique based on divide and conquer technique. With worst-case time complexity being Ο(n log n), it is one of the most respected algorithms.

https://www.youtube.com/watch?v=GCae1WNvnZM

Merge sort first divides the array into equal halves and then combines them in a sorted manner.

![Merge Sort](/../master/images/mergesort1.png?raw=true "Merge sort ex")
![Merge Sort](/../master/images/mergesort2.png?raw=true "Merge sort continued ex")

Merge sort keeps on dividing the list into equal halves until it can no more be divided. By definition, if it is only one element in the list, it is sorted. Then, merge sort combines the smaller sorted lists keeping the new list sorted too.

Step 1 − if it is only one element in the list it is already sorted, return.
Step 2 − divide the list recursively into two halves until it can no more be divided.
Step 3 − merge the smaller lists into new list in sorted order.

Pseudocode
We shall now see the pseudocodes for merge sort functions. As our algorithms point out two main functions − divide & merge.
Merge sort works with recursion and we shall see our implementation in the same way.

procedure mergesort( var a as array )
   if ( n == 1 ) return a

   var l1 as array = a[0] ... a[n/2]
   var l2 as array = a[n/2+1] ... a[n]

   l1 = mergesort( l1 )
   l2 = mergesort( l2 )

   return merge( l1, l2 )
end procedure

procedure merge( var a as array, var b as array )

   var c as array

   while ( a and b have elements )
      if ( a[0] > b[0] )
         add b[0] to the end of c
         remove b[0] from b
      else
         add a[0] to the end of c
         remove a[0] from a
      end if
   end while
   
   while ( a has elements )
      add a[0] to the end of c
      remove a[0] from a
   end while
   
   while ( b has elements )
      add b[0] to the end of c
      remove b[0] from b
   end while
   
   return c
	
end procedure

```python 
def mergeSort(alist):
    print("Splitting ",alist)
    if len(alist)>1:
        mid = len(alist)//2
        lefthalf = alist[:mid]
        righthalf = alist[mid:]

        mergeSort(lefthalf)
        mergeSort(righthalf)

        i=0
        j=0
        k=0
        while i < len(lefthalf) and j < len(righthalf):
            if lefthalf[i] < righthalf[j]:
                alist[k]=lefthalf[i]
                i=i+1
            else:
                alist[k]=righthalf[j]
                j=j+1
            k=k+1

        while i < len(lefthalf):
            alist[k]=lefthalf[i]
            i=i+1
            k=k+1

        while j < len(righthalf):
            alist[k]=righthalf[j]
            j=j+1
            k=k+1
    print("Merging ",alist)

alist = [54,26,93,17,77,31,44,55,20]
mergeSort(alist)
print(alist)

```

#### tim sort:
Timsort is a hybrid algorithm combining merge and insertion sorts. It was invented in 2002 by Tim Peters for use in the Python programming language.

Timsort begins by looking for small runs (called miniruns) of data that is already sorted (either small to large or vice versa). If no runs of a long-enough length (minirun is short for “minimum run”) are found then an insertion step takes place to create them. These miniruns are sorted using an insertion sort as explained above, and then combined with each other using Mergesort, again as explained above. The trick of Timsort is how it selects and creates miniruns,

![Tim Sort](/../master/images/timsort1.png?raw=true "Tim sort info")

![Tim Sort ...](/../master/images/timsort2.png?raw=true "Tim sort continued")

"""
|-----------------------------------------------------------|
|                         Tim Sort                          |
|-----------------------------------------------------------|
|Created By Tim Roberts      ||  Python's Builtin Sort      |
|Worst case performance      ||  O(n log n)                 |
|Best case performance       ||  O(n)                       |
|Average case performance    ||  O(n log n)                 |
|Worst case space complexity ||  O(n)                       |
|-----------------------------------------------------------|
"""

```python 
def tim_sort(array): 
	return sorted(array)"
```

#### heapsort:
heaps are based on the notion of complete tree
2 main steps are
creation of heap
processing of heap

creation of heap = put the largest element on the top in each step size of the Heap increase
put the elements in heap as they occur in swquential order i.e, parent then leftchild and then right child.
Each time (of inserting a new node/child) compare it with its Parent Node
Is? Child Node (left or Right ) is less than or equal or greater than or equal to that of the parent node ...
If it is greater than its successor than interchange them. 

processing of the heap:
The last element of last level should be replsced by ztop element of the list.
zin this process the largest element will be at last place
List weill be sdorted out in increasing order from root to leaf
After reolacing the top element Creatuion of the Heap takes place 
In each stepo size of Heap decreased

![Heap Sort](/../master/images/heapsort0.png?raw=true "heap sort ex")
![Heap Sort](/../master/images/heapsort1.png?raw=true "heap sort continued")
![Heap Sort](/../master/images/heapsort2.png?raw=true "heap sort continued")
![Heap Sort](/../master/images/heapsort3.png?raw=true "heap sort continued")
![Heap Sort](/../master/images/heapsort4.png?raw=true "heap sort continued")
![Heap Sort](/../master/images/heapsort5.png?raw=true "heap sort continued")
![Heap Sort](/../master/images/heapsort6.png?raw=true "heap sort continued")

https://www.youtube.com/watch?v=onlhnHpGgC4

```python
#  Statement:
#  Given a disordered list of integers (or any other items),
#  rearrange the integers in natural order.
#
#  Sample Input: [8,5,3,1,9,6,0,7,4,2,5]
#  Sample Output: [0,1,2,3,4,5,5,6,7,8,9]
#
#  Time Complexity of Solution:
#  Best O(nlog(n)); Average O(nlog(n)); Worst O(nlog(n)).
#
#  Approach:
#  Heap sort happens in two phases. In the first phase, the array
#  is transformed into a heap. A heap is a binary tree where
#  1) each node is greater than each of its children
#  2) the tree is perfectly balanced
#  3) all leaves are in the leftmost position available.
#  In phase two the heap is continuously reduced to a sorted array:
#  1) while the heap is not empty
#  - remove the top of the head into an array
#  - fix the heap.
#  Heap sort was invented by John Williams not by B. R. Heap.
#
#  MoveDown:
#  The movedown method checks and verifies that the structure is a heap.
#
#  Technical Details:
#  A heap is based on an array just as a hashmap is based on an
#  array. For a heap, the children of an element n are at index
#  2n+1 for the left child and 2n+2 for the right child.
#
#  The movedown function checks that an element is greater than its
#  children. If not the values of element and child are swapped. The
#  function continues to check and swap until the element is at a
#  position where it is greater than its children.
#======================================================================= 
 def heapsort( aList ):
  # convert aList to heap
  length = len( aList ) - 1
  leastParent = length / 2
  for i in range ( leastParent, -1, -1 ):
    moveDown( aList, i, length )
 
  # flatten heap into sorted array
  for i in range ( length, 0, -1 ):
    if aList[0] > aList[i]:
      swap( aList, 0, i )
      moveDown( aList, 0, i - 1 )
 
def moveDown( aList, first, last ):
  largest = 2 * first + 1
  while largest <= last:
    # right child exists and is larger than left child
    if ( largest < last ) and ( aList[largest] < aList[largest + 1] ):
      largest += 1
 
    # right child is larger than parent
    if aList[largest] > aList[first]:
      swap( aList, largest, first )
      # move down to largest child
      first = largest;
      largest = 2 * first + 1
    else:
      return # force exit
  
def swap( A, x, y ):
  tmp = A[x]
  A[x] = A[y]
  A[y] = tmp
```

#### buble sort:
	Bubble sort is one of the most basic sorting algorithm that is the simplest to understand. It’s basic idea is to bubble up the largest(or smallest), then the 2nd largest and the the 3rd and so on to the end of the list. Each bubble up takes a full sweep through the list

https://www.youtube.com/watch?v=P00xJgWzz2c

![Bubble Sort](/../master/images/bubblesort.png?raw=true "Bubble sort ex")

```python 
def bubble_sort(items):
        """ Implementation of bubble sort """
        for i in range(len(items)):
                for j in range(len(items)-1-i):
                        if items[j] &gt; items[j+1]:
                                items[j], items[j+1] = items[j+1], items[j]     # Swap!
 
```

#### insertion sort:
The insertion sort uses the principle of a marker moving along a list with a sorted side to the left side of the marker and the unsorted side to the right of the marker.

![Insertion sort](/../master/images/insertionsort.png?raw=true "Insertion sort ex")

http://courses.cs.vt.edu/csonline/Algorithms/Lessons/InsertionCardSort/insertioncardsort.html
https://www.youtube.com/watch?v=Nkw6Jg_Gi4w


ex. 
![Insertion sort](/../master/images/insertionsort1.png?raw=true "Insertion sort ex")
![Insertion sort](/../master/images/insertionsort2.png?raw=true "Insertion sort ex")
![Insertion sort](/../master/images/insertionsort3.png?raw=true "Insertion sort ex")
![Insertion sort](/../master/images/insertionsort4.png?raw=true "Insertion sort ex")
![Insertion sort](/../master/images/insertionsort5.png?raw=true "Insertion sort ex")
![Insertion sort](/../master/images/insertionsort6.png?raw=true "Insertion sort ex")

```python 
def insertionSort(alist):
   for index in range(1,len(alist)):

     currentvalue = alist[index]
     position = index

     while position>0 and alist[position-1]>currentvalue:
         alist[position]=alist[position-1]
         position = position-1

     alist[position]=currentvalue

alist = [54,26,93,17,77,31,44,55,20]
insertionSort(alist)
print(alist)
```

#### selection sort:
The selection sort improves on the bubble sort by making only one exchange for every pass through the list. In order to do this, a selection sort looks for the largest value as it makes a pass and, after completing the pass, places it in the proper location. As with a bubble sort, after the first pass, the largest item is in the correct place. After the second pass, the next largest is in place. This process continues and requires n−1n−1 passes to sort n items, since the final item must be in place after the (n−1)(n−1) st pass.


![Selection Sort](/../master/images/selectionsort.png?raw=true "Selection sort ex")

https://www.youtube.com/watch?v=6nDMgr0-Yyo

```python 
def selectionSort(alist):
   for fillslot in range(len(alist)-1,0,-1):
       positionOfMax=0
       for location in range(1,fillslot+1):
           if alist[location]>alist[positionOfMax]:
               positionOfMax = location

       temp = alist[fillslot]
       alist[fillslot] = alist[positionOfMax]
       alist[positionOfMax] = temp

alist = [54,26,93,17,77,31,44,55,20]
selectionSort(alist)
print(alist)
```

#### tree sort:
Tree sort is a sorting algorithm that is based on Binary Search Tree data structure. It first creates a binary search tree from the elements of the input list or array and then performs an in-order traversal on the created binary search tree to get the elements in sorted order.

![Tree Sort](/../master/images/treesort0.png?raw=true "tree sort ex")
![Tree Sort...](/../master/images/treesort1.png?raw=true "tree sort continued")
![Tree Sort...](/../master/images/treesort2.png?raw=true "tree sort continued")
![Tree Sort...](/../master/images/treesort3.png?raw=true "tree sort continued")

https://www.youtube.com/watch?v=pYT9F8_LFTM

Algorithm:

Step 1: Take the elements input in an array.
Step 2: Create a Binary search tree by inserting data items from the array into the
        binary searh tree.
Step 3: Perform in-order traversal on the tree to get the elements in sorted order.

```python 
   class Node:
      def __init__(self,info): #constructor of class
          self.info = info  #information for node
          self.left = None  #left leef
          self.right = None #right leef
          self.level = None #level none defined
      def __str__(self):
          return str(self.info) #return as string

class searchtree:
      def __init__(self): #constructor of class
          self.root = None

      def create(self,val):  #create binary search tree nodes
          if self.root == None:
             self.root = Node(val)
          else:
             current = self.root
             while 1:
                 if val < current.info:
                   if current.left:
                      current = current.left
                   else:
                      current.left = Node(val)
                      break;      

                 elif val > current.info:
                    if current.right:
                       current = current.right
                    else:
                       current.right = Node(val)
                       break;      
                 else:
                    break 

      def bft(self): #Breadth-First Traversal
          self.root.level = 0 
          queue = [self.root]
          out = []
          current_level = self.root.level
          while len(queue) > 0:
             current_node = queue.pop(0)
             if current_node.level > current_level:
                current_level += 1
                out.append("\n")
             out.append(str(current_node.info) + " ")
             if current_node.left:
                current_node.left.level = current_level + 1
                queue.append(current_node.left)  
             if current_node.right:
                current_node.right.level = current_level + 1
                queue.append(current_node.right)
             print "".join(out)   

      def inorder(self,node):
           if node is not None:              
              self.inorder(node.left)
              print node.info
              self.inorder(node.right)

      def preorder(self,node):
           if node is not None:
              print node.info
              self.preorder(node.left)
              self.preorder(node.right)

      def postorder(self,node):
           if node is not None:
              self.postorder(node.left)
              self.postorder(node.right)
              print node.info

tree = searchtree()     
arr = [8,3,1,6,4,7,10,14,13]
for i in arr:
    tree.create(i)
print 'Breadth-First Traversal'
tree.bft()
print 'Inorder Traversal'
tree.inorder(tree.root) 
print 'Preorder Traversal'
tree.preorder(tree.root) 
print 'Postorder Traversal'
tree.postorder(tree.root) 
```

#### shell sort:
"""
|-----------------------------------------------------------|
|                        Shell Sort                         |
|-----------------------------------------------------------|
|Type                        ||  In-place comparison sort   |
|Worst case performance      ||  O(n2)                      |
|Best case performance       ||  O(n log2 n)                |
|Average case performance    ||  depends on gap sequence    |
|Worst case space complexity ||  O(n) total, O(1) auxiliary |
|-----------------------------------------------------------|
"""


![Shell Sort](/../master/images/shellsort.png?raw=true "shell sort ex")
https://www.youtube.com/watch?v=M9YCh-ZeC7Y

```python
def shell_sort(array):
    mid = len(array) / 2
    while mid:
        for i in range(len(array)):
            j = i
            temp = array[i]
            while j >= mid and array[j-mid] > temp:
                array[j] = array[j - mid]
                j -= mid
            array[j] = temp
        mid = mid/2 if mid/2 else (0 if mid == 1 else 1)
    return array
```

#### bucket sort:
Bucket sort can be exceptionally fast because of the way elements are assigned to buckets, typically using an array where the index is the value. This means that more auxiliary memory is required for the buckets at the cost of running time than more comparison sorts. It runs in O(n+k)O(n+k) time in the average case where nn is the number of elements to be sorted and kk is the number of buckets.

![Bucket Sort](/../master/images/bucketsort.png?raw=true "bucket sort ex")
![Bucket Sort Complexity](/../master/images/bucketsortcomplexity.png?raw=true "bucket sort complexity")

when its fast:
Bucket sort’s best case occurs when the data being sorted can be distributed between the buckets perfectly. If the values are sparsely allocated over the possible value range, a larger bucket size is better since the buckets will likely be more evenly distributed. An example of this is [2303, 33, 1044], if buckets can only contain 5 different values then for this example 461 buckets would need to be initialised. A bucket size of 200-1000 would be much more reasonable.
The inverse of this is also true; a densely allocated array like [103, 99, 119, 112, 111] performs best when buckets are as small as possible.
Bucket sort is an ideal algorithm choice when:
The additional O(n + k)O(n+k) memory usage is not an issue
Elements are expected to be fairly evenly distributed

when its slow:
Bucket sort performs at its worst, O(n^2), when all elements at allocated to the same bucket. Since individual buckets are sorted using another algorithm, if only a single bucket needs to be sorted, bucket sort will take on the complexity of the inner sorting algorithm.
This depends on the individual implementation though and can be mitigated. For example a bucket sort algorithm could be made to work with large bucket sizes by using insertion sort on small buckets (due to its low overhead), and merge sort or quicksort on larger buckets.

```python 
def sort(array, bucketSize=DEFAULT_BUCKET_SIZE):
  if len(array) == 0:
    return array

  # Determine minimum and maximum values
  minValue = array[0]
  maxValue = array[0]
  for i in range(1, len(array)):
    if array[i] < minValue:
      minValue = array[i]
    elif array[i] > maxValue:
      maxValue = array[i]

  # Initialize buckets
  bucketCount = math.floor((maxValue - minValue) / bucketSize) + 1
  buckets = []
  for i in range(0, bucketCount):
    buckets.append([])

  # Distribute input array values into buckets
  for i in range(0, len(array)):
    buckets[math.floor((array[i] - minValue) / bucketSize)].append(array[i])

  # Sort buckets and place back into input array
  array = []
  for i in range(0, len(buckets)):
    insertion_sort.sort(buckets[i])
    for j in range(0, len(buckets[i])):
      array.append(buckets[i][j])

  return array
  ```

#### radix sort:
The algorithm is named radix sort as it specifies the radix rr to be used which changes how the sort is performed. The radix, or base, of the number system is the number of digits that represent a single position in the number; a radix of 2 is binary (0-1), 10 is decimal (0-9), 16 is hexadecimal (0-F) and so on. Since the radix determines the number of buckets in addition to the word size ww used in the algorithm, changing it can drastically change how the sort plays out:

https://www.youtube.com/watch?v=YXFI4osELGU

![Radix Sort](/../master/images/radixsort1.png?raw=true "radix sort ex")
![Radix Sort](/../master/images/radixsort2.png?raw=true "radix sort ex")
![Radix Sort](/../master/images/radixsort3.png?raw=true "radix sort ex")
![Radix Sort](/../master/images/radixsort4.png?raw=true "radix sort ex")
![Radix Sort](/../master/images/radixsort5.png?raw=true "radix sort ex")
![Radix Sort](/../master/images/radixsort6.png?raw=true "radix sort ex")
![Radix Sort](/../master/images/radixsort7.png?raw=true "radix sort ex")
![Radix Sort](/../master/images/radixsort8.png?raw=true "radix sort ex")
![Radix Sort](/../master/images/radixsort9.png?raw=true "radix sort ex")
![Radix Sort](/../master/images/radixsort10.png?raw=true "radix sort ex")
![Radix Sort](/../master/images/radixsort11.png?raw=true "radix sort ex")
![Radix Sort](/../master/images/radixsort12.png?raw=true "radix sort ex")
![Radix Sort](/../master/images/radixsort13.png?raw=true "radix sort ex")

fast: 
Since comparison sorts cannot perform better than O(n\log n)O(nlogn), LSD radix sort is considered one of the best alternatives provided the word size ww is expected to be less than \log nlogn.
It does however have limitations on the type of keys that can be sorted in that they need to have some way of being split up (ie. the radix), so it’s typically only used for string (where r=255r=255 for ASCII characters) and integer keys.

slow:
Radix sort is slowest when the word size is large, such as when there is a large key range but small radix. The reason that a large radix is not always used is because then it essentially becomes counting sort, with the large memory footprint associated with it.

```python
def sort(array, radix=10):
  if len(array) == 0:
    return array

  # Determine minimum and maximum values
  minValue = array[0];
  maxValue = array[0];
  for i in range(1, len(array)):
    if array[i] < minValue:
      minValue = array[i]
    elif array[i] > maxValue:
      maxValue = array[i]

  # Perform counting sort on each exponent/digit, starting at the least
  # significant digit
  exponent = 1
  while (maxValue - minValue) / exponent >= 1:
    array = countingSortByDigit(array, radix, exponent, minValue)
    exponent *= radix

  return array

def countingSortByDigit(array, radix, exponent, minValue):
  bucketIndex = -1
  buckets = [0] * radix
  output = [None] * len(array)

  # Count frequencies
  for i in range(0, len(array)):
    bucketIndex = math.floor(((array[i] - minValue) / exponent) % radix)
    buckets[bucketIndex] += 1

  # Compute cumulates
  for i in range(1, radix):
    buckets[i] += buckets[i - 1]

  # Move records
  for i in range(len(array) - 1, -1, -1):
    bucketIndex = math.floor(((array[i] - minValue) / exponent) % radix)
    buckets[bucketIndex] -= 1
    output[buckets[bucketIndex]] = array[i]

  return output
```

#### counting sort:
	is a sorting technique based on keys between a specific range
	it works by counting the number of objects having distinct key values
	Then doing some arithmetic to calculate the position of each object in the output sequence
	
	Time Complexity: O(n+k) where n is the number of elements in input array and k is the range of input.
Auxiliary Space: O(n+k)

Points to be noted:
Counting sort is efficient if the range of input data is not significantly greater than the number of objects to be sorted. Consider the situation where the input sequence is between range 1 to 10K and the data is 10, 5, 10K, 5K.
It is not a comparison based sorting. It running time complexity is O(n) with space proportional to the range of data.
It is often used as a sub-routine to another sorting algorithm like radix sort.
Counting sort uses a partial hashing to count the occurrence of the data object in O(1).
Counting sort can be extended to work for negative inputs also.
	
https://www.youtube.com/watch?v=7zuGmKfUt7s
	
![Counting Sort](/../master/images/countingsort1.png?raw=true "counting sort ex")	
![Counting Sort](/../master/images/countingsort2.png?raw=true "counting sort ex")
![Counting Sort](/../master/images/countingsort3.png?raw=true "counting sort ex")
![Counting Sort](/../master/images/countingsort4.png?raw=true "counting sort ex")
![Counting Sort](/../master/images/countingsort5.png?raw=true "counting sort ex")
![Counting Sort](/../master/images/countingsort6.png?raw=true "counting sort ex")
![Counting Sort](/../master/images/countingsort7.png?raw=true "counting sort ex")
![Counting Sort](/../master/images/countingsort8.png?raw=true "counting sort ex")
![Counting Sort](/../master/images/countingsort9.png?raw=true "counting sort ex")
![Counting Sort](/../master/images/countingsort10.png?raw=true "counting sort ex")
![Counting Sort](/../master/images/countingsort11.png?raw=true "counting sort ex")
![Counting Sort](/../master/images/countingsort12.png?raw=true "counting sort ex")

```python
def counting_sort(array, maxval):
    """in-place counting sort"""
    m = maxval + 1
    count = [0] * m               # init with zeros
    for a in array:
        count[a] += 1             # count occurences
    i = 0
    for a in range(m):            # emit
        for c in range(count[a]): # - emit 'count[a]' copies of 'a'
            array[i] = a
            i += 1
    return (array,count)

print counting_sort( [1, 4, 7, 2, 1, 3, 2, 1, 4, 2, 3, 2, 1], 7 )
#            prints: [1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 4, 4, 7],[0, 4, 4, 2, 2, 0, 0, 1]
```

#### cubesort sort:
Cubesort is a parallel sorting algorithm that builds a self-balancing multi-dimensional array from the keys to be sorted. As the axes are of similar length the structure resembles a cube. After each key is inserted the cube can be rapidly converted to an arra
Data structure : array
worst-case preformance: O(n log n)
worst-case space complexity: 0(n)
operation: Cubesort's algorithm uses a specialized binary search on each axis to find the location to insert an element. When an axis grows too large it is split. Locality of reference is optimal as only 4 binary searches are performed on small arrays for each insertion. By using many small dynamic arrays the high cost for insertion on single large arrays is avoided.

```c
/*
	Copyright 2014 Gregorius van den Hoven - gregoriusvandenhoven@gmail.com
*/

/*
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
	Binary Search Tesseract v1.0
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define BSC_M 64

#define BSC_X 128
#define BSC_Y 64
#define BSC_Z 64

struct cube
{
	int *w_floor;
	struct w_node **w_axis;
	unsigned char *x_size;
	unsigned short w_size;
};

struct w_node
{
	int x_floor[BSC_X];
	struct x_node *x_axis[BSC_X];
	unsigned char y_size[BSC_X];
};

struct x_node
{
	int y_floor[BSC_Y];
	struct y_node *y_axis[BSC_Y];
	unsigned char z_size[BSC_Y];
};

struct y_node
{
	int z_keys[BSC_Z];
};

void split_w_node(struct cube *cube, short w);
void split_x_node(struct cube *cube, short w, short x);
void split_y_node(struct w_node *w_node, short x, short y);

struct cube *create_cube(void)
{
	struct cube *cube;

	cube = (struct cube *) calloc(1, sizeof(struct cube));

	return cube;
}

void destroy_cube(struct cube *cube, int *z_array)
{
	if (cube->w_size)
	{
		struct w_node *w_node;
		struct x_node *x_node;
		struct y_node *y_node;

		register short w, x, y;

		register int z_size = 0;

		for (w = 0 ; w < cube->w_size ; ++w)
		{
			w_node = cube->w_axis[w];

			for (x = 0 ; x < cube->x_size[w] ; ++x)
			{
				x_node = w_node->x_axis[x];
			
				for (y = 0 ; y < w_node->y_size[x] ; ++y)
				{
					y_node = x_node->y_axis[y];

					memmove(&z_array[z_size], &y_node->z_keys[0], x_node->z_size[y] * sizeof(int));

					z_size += x_node->z_size[y];

					free(y_node);
				}
				free(x_node);
			}
			free(w_node);
		}
		free(cube->w_floor);
		free(cube->w_axis);
		free(cube->x_size);
	}
	free(cube);
}

inline void insert_z_node(struct cube *cube, int key)
{
	struct w_node *w_node;
	struct x_node *x_node;
	struct y_node *y_node;

	register short mid, w, x, y, z;

	if (cube->w_size == 0)
	{
		cube->w_floor = (int *) malloc(BSC_M * sizeof(int));
		cube->w_axis = (struct w_node **) malloc(BSC_M * sizeof(struct w_node *));
		cube->x_size = (unsigned char *) malloc(BSC_M * sizeof(unsigned char));

		w_node = cube->w_axis[0] = (struct w_node *) malloc(sizeof(struct w_node));

		x_node = w_node->x_axis[0] = (struct x_node *) malloc(sizeof(struct x_node));

		y_node = x_node->y_axis[0] = (struct y_node *) malloc(sizeof(struct y_node));

		x_node->z_size[0] = 0;

		cube->w_size = cube->x_size[0] = w_node->y_size[0] = 1;

		w = x = y = z = 0;
		
		cube->w_floor[0] = w_node->x_floor[0] = x_node->y_floor[0] = key;

		goto insert;
	}

	if (key < cube->w_floor[0])
	{
		w_node = cube->w_axis[0];
		x_node = w_node->x_axis[0];
		y_node = x_node->y_axis[0];

		w = x = y = z = 0;

		cube->w_floor[0] = w_node->x_floor[0] = x_node->y_floor[0] = key;

		goto insert;
	}

	// w

	mid = w = cube->w_size - 1;

	while (mid > 7)
	{
		mid /= 2;

		if (key < cube->w_floor[w - mid]) w -= mid;
	}
	while (key < cube->w_floor[w]) --w;

	w_node = cube->w_axis[w];

	// x

	mid = x = cube->x_size[w] - 1;

	while (mid > 7)
	{
		mid /= 2;

		if (key < w_node->x_floor[x - mid])
		{
			x -= mid;
		}
	}
	while (key < w_node->x_floor[x]) --x;

	x_node = w_node->x_axis[x];

	// y

	mid = y = w_node->y_size[x] - 1;

	while (mid > 7)
	{
		mid /= 4;

		if (key < x_node->y_floor[y - mid])
		{
			y -= mid;
			if (key < x_node->y_floor[y - mid])
			{
				y -= mid;
				if (key < x_node->y_floor[y - mid])
				{
					y -= mid;
				}
			}
		}
	}
	while (key < x_node->y_floor[y]) --y;

	y_node = x_node->y_axis[y];

	// z

	mid = z = x_node->z_size[y] - 1;

	while (mid > 7)
	{
		mid /= 4;

		if (key < y_node->z_keys[z - mid])
		{
			z -= mid;
			if (key < y_node->z_keys[z - mid])
			{
				z -= mid;
				if (key < y_node->z_keys[z - mid])
				{
					z -= mid;
				}
			}
		}
	}
	while (key < y_node->z_keys[z]) --z;


/*
//	Uncomment to filter duplicates

	if (key == y_node->z_keys[z])
	{
		return;
	}
*/
	++z;

	insert:

	++x_node->z_size[y];

	if (z + 1 != x_node->z_size[y])
	{
		memmove(&y_node->z_keys[z + 1], &y_node->z_keys[z], (x_node->z_size[y] - z - 1) * sizeof(int));
	}

	y_node->z_keys[z] = key;

	if (x_node->z_size[y] == BSC_Z)
	{
		split_y_node(w_node, x, y);

		if (w_node->y_size[x] == BSC_Y)
		{
			split_x_node(cube, w, x);

			if (cube->x_size[w] == BSC_X)
			{
				split_w_node(cube, w);
			}
		}
	}
}

inline void insert_w_node(struct cube *cube, short w)
{
	++cube->w_size;

	if (cube->w_size % BSC_M == 0)
	{
		cube->w_floor = (int *) realloc(cube->w_floor, (cube->w_size + BSC_M) * sizeof(int));
		cube->w_axis = (struct w_node **) realloc(cube->w_axis, (cube->w_size + BSC_M) * sizeof(struct w_node *));
		cube->x_size = (unsigned char *) realloc(cube->x_size, (cube->w_size + BSC_M) * sizeof(unsigned char));
	}

	if (w + 1 != cube->w_size)
	{
		memmove(&cube->w_floor[w + 1], &cube->w_floor[w], (cube->w_size - w - 1) * sizeof(int));
		memmove(&cube->w_axis[w + 1], &cube->w_axis[w], (cube->w_size - w - 1) * sizeof(struct w_node *));
		memmove(&cube->x_size[w + 1], &cube->x_size[w], (cube->w_size - w - 1) * sizeof(unsigned char));
	}

	cube->w_axis[w] = (struct w_node *) malloc(sizeof(struct w_node));
}

void split_w_node(struct cube *cube, short w)
{
	struct w_node *w_node1, *w_node2;

	insert_w_node(cube, w + 1);

	w_node1 = cube->w_axis[w];
	w_node2 = cube->w_axis[w + 1];

	cube->x_size[w + 1] = cube->x_size[w] / 2;
	cube->x_size[w] -= cube->x_size[w + 1];

	memcpy(&w_node2->x_floor[0], &w_node1->x_floor[cube->x_size[w]], cube->x_size[w + 1] * sizeof(int));
	memcpy(&w_node2->x_axis[0], &w_node1->x_axis[cube->x_size[w]], cube->x_size[w + 1] * sizeof(struct x_node *));
	memcpy(&w_node2->y_size[0], &w_node1->y_size[cube->x_size[w]], cube->x_size[w + 1] * sizeof(unsigned char));

	cube->w_floor[w + 1] = w_node2->x_floor[0];
}

inline void insert_x_node(struct cube *cube, short w, short x)
{
	struct w_node *w_node = cube->w_axis[w];

	short x_size = ++cube->x_size[w];

	if (x_size != x + 1)
	{
		memmove(&w_node->x_floor[x + 1], &w_node->x_floor[x], (x_size - x - 1) * sizeof(int));
		memmove(&w_node->x_axis[x + 1], &w_node->x_axis[x], (x_size - x - 1) * sizeof(struct x_node *));
		memmove(&w_node->y_size[x + 1], &w_node->y_size[x], (x_size - x - 1) * sizeof(unsigned char));
	}

	w_node->x_axis[x] = (struct x_node *) malloc(sizeof(struct x_node));
}

void split_x_node(struct cube *cube, short w, short x)
{
	struct w_node *w_node = cube->w_axis[w];
	struct x_node *x_node1, *x_node2;

	insert_x_node(cube, w, x + 1);

	x_node1 = w_node->x_axis[x];
	x_node2 = w_node->x_axis[x + 1];

	w_node->y_size[x + 1] = w_node->y_size[x] / 2;
	w_node->y_size[x] -= w_node->y_size[x + 1];

	memcpy(&x_node2->y_floor[0], &x_node1->y_floor[w_node->y_size[x]], w_node->y_size[x + 1] * sizeof(int));
	memcpy(&x_node2->y_axis[0], &x_node1->y_axis[w_node->y_size[x]], w_node->y_size[x + 1] * sizeof(struct y_node *));
	memcpy(&x_node2->z_size[0], &x_node1->z_size[w_node->y_size[x]], w_node->y_size[x + 1] * sizeof(unsigned char));

	w_node->x_floor[x + 1] = x_node2->y_floor[0];
}

inline void insert_y_node(struct w_node *w_node, short x, short y)
{
	struct x_node *x_node = w_node->x_axis[x];

	short y_size = ++w_node->y_size[x];

	if (y_size != y + 1)
	{
		memmove(&x_node->y_floor[y + 1], &x_node->y_floor[y], (y_size - y - 1) * sizeof(int));
		memmove(&x_node->y_axis[y + 1], &x_node->y_axis[y], (y_size - y - 1) * sizeof(struct y_node *));
		memmove(&x_node->z_size[y + 1], &x_node->z_size[y], (y_size - y - 1) * sizeof(unsigned char));
	}

	x_node->y_axis[y] = (struct y_node *) malloc(sizeof(struct y_node));
}

void split_y_node(struct w_node *w_node, short x, short y)
{
	struct x_node *x_node = w_node->x_axis[x];
	struct y_node *y_node1, *y_node2;

	insert_y_node(w_node, x, y + 1);

	y_node1 = x_node->y_axis[y];
	y_node2 = x_node->y_axis[y + 1];

	x_node->z_size[y + 1] = x_node->z_size[y] / 2;
	x_node->z_size[y] -= x_node->z_size[y + 1];

	memcpy(&y_node2->z_keys[0], &y_node1->z_keys[x_node->z_size[y]], x_node->z_size[y + 1] * sizeof(int));

	x_node->y_floor[y + 1] = y_node2->z_keys[0];
}

long long utime()
{
	struct timeval now_time;

	gettimeofday(&now_time, NULL);

	return now_time.tv_sec * 1000000LL + now_time.tv_usec;
}

void cubesort(int *array, int size)
{
	struct cube *cube;
	int cnt;

	if (size > 1000000)
	{
		for (cnt = 100000 ; cnt + 100000 < size ; cnt += 100000)
		{
			cubesort(&array[cnt], 100000);
		}

		cubesort(&array[cnt], size - cnt);
	}

	cube = create_cube();

	for (cnt = 0 ; cnt < size ; cnt++)
	{
		insert_z_node(cube, array[cnt]);
	}
	destroy_cube(cube, array);
}

int main(int argc, char **argv)
{
	static int max = 10000000;
	int cnt;
	long long start, end;
	int *z_array;

	if (argv[1] && *argv[1])
	{
		printf("%s\n", argv[1]);
	}

	z_array = malloc(max * sizeof(int));

	for (cnt = 0 ; cnt < max ; cnt++)
	{
		z_array[cnt] = rand();
	}

	start = utime();

	cubesort(z_array, max);

	end = utime();

	printf("Cubesort: sorted %d elements in %f seconds. (random order)\n", max, (end - start) / 1000000.0);

	for (cnt = 0 ; cnt < max ; cnt++)
	{
		z_array[cnt] = cnt;
	}

	start = utime();

	cubesort(z_array, max);

	end = utime();

	printf("Cubesort: sorted %d elements in %f seconds. (forward order)\n", max, (end - start) / 1000000.0);

	for (cnt = 0 ; cnt < max ; cnt++)
	{
		z_array[cnt] = max - cnt;
	}

	start = utime();

	cubesort(z_array, max);

	end = utime();

	printf("Cubesort: sorted %d elements in %f seconds. (reverse order)\n", max, (end - start) / 1000000.0);

	for (cnt = 1 ; cnt < max ; cnt++)
	{
		if (z_array[cnt - 1] > z_array[cnt])
		{
			printf("Cubesort: not properly sorted at index %d. (%d vs %d\n", cnt, z_array[cnt - 1], z_array[cnt]);
			return 0;
		}
	}

	return 0;
}
```

* Know all the (n log(n)) sorting algorithm
worst case = merge sort, timsort, heapsort, cubesort 
average = quick sort , merge sort, timsort, heapsort, tree sort, cube sort
best = quicksort, merge sort, heap sort, tree sort, shell sort

* know these for sure:quicksort and merge sort
see above (reminder) 
quicksort = Quick sort is a highly efficient sorting algorithm and is based on partitioning of array of data into smaller arrays. A large array is partitioned into two arrays one of which holds values smaller than the specified value, say pivot, based on which the partition is made and another array holds values greater than the pivot value.

Quick sort partitions an array and then calls itself recursively twice to sort the two resulting subarrays. This algorithm is quite efficient for large-sized data sets as its average and worst case complexity are of Ο(nlogn), where n is the number of items.
 
mergersort = Merge sort is a sorting technique based on divide and conquer technique. With worst-case time complexity being Ο(n log n), it is one of the most respected algorithms.

Merge sort first divides the array into equal halves and then combines them in a sorted manner.

* know why Merge sort can be highly useful in situations where quicksort is impractical.
Take mergesort over quicksort if you don't know anything about the data. Merge Sort Worst case complexity is O(nlogn) whereas Quick Sort worst case is O(n^2). Merge Sort is a stable sort which means that the same element in an array maintain their original positions with respect to each other

Quicksort depends on being able to index into an array or similar structure. When that's possible, it's hard to beat Quicksort.

But you can't index directly into a linked list very quickly. That is, if myList is a linked list, then myList[x], were it possible to write such syntax, would involve starting at the head of the list and following the first x links. That would have to be done twice for every comparison that Quicksort makes, and that would get expensive real quick.

Same thing on disk: Quicksort would have to seek and read every item it wants to compare.

Merge sort is faster in these situations because it reads the items sequentially, typically making log2(N) passes over the data. There is much less I/O involved, and much less time spent following links in a linked list.

Quicksort is fast when the data fits into memory and can be addressed directly. Mergesort is faster when data won't fit into memory or when it's expensive to get to an item.


## 7. hashtables (must know or will fail)
* Be able to implement one using only arrays in your favorite language, in about the space of one interview.
python example:

#### hash Table:
are used wehn speedy insertion, deletion, and lookup is the priority. In fact, for an ideally tuned hash table, insertion, deletion, and lookup can be accomplished in constant time

A has table is an array associated with a function(the hash function)

components of a hashmap
	Array
	Hash function
	collision handling

![Hash Table](/../master/images/hashtable1.png?raw=true "hash table ex")

hash function = maps keys to array indices. For example, in this slide we see that the hash function has mapped the key 'banana' to index 1.
But what's going on under the hood?
The hash function takes a key as input and computes an array index from the intrinsic properties of that key.
You’d initially use the hash function to determine where in the hash table to store a given key. Later, you’d use the same hash function to determine where in the hash table to search for a given key. For this reason, it’s crucial that a hash function behaves consistently and outputs the same index for identical inputs.

![Hash Table](/../master/images/hashtable2.png?raw=true "hash table ex")

Keep in mind that hash tables can be used to store data of all types, but for now, let’s consider a very simple hash function for strings.

This hash function uses the first letter of a string to determine a hash table index for that string, so words that start with the letter 'a' are assigned to index 0, 'b' to index 1, and so on.

Note that this hash function returns hash % SIZE, where SIZE is the size of the hash table. Modding by the size of the hash table is a good way to avoid indexing into a hash table slot that does not exist.

![Hash Table](/../master/images/hashtable3.png?raw=true "hash table ex")

Time to throw a wrench into things. What if we want to store the word berry into the table as well?

Berry hashes to index 1, just as banana did. This is an example of a collision, the result of two keys hashing to the same index.

Even if your hash table is larger than your dataset and you’ve chosen a good hash function, you need a plan for dealing with collisions if and when they arise. Two common methods of dealing with collisions are linear probing and separate chaining.

![Hash Table](/../master/images/hashtable4.png?raw=true "hash table ex")

With linear probing, if a key hashes to the same index as a previously stored key, it is assigned the next available slot in the table.

So banana is stored at index 1, and berry is stored at index 3.

As you can surmise even from this simple example, once a collision occurs, you’ve significantly increased chances that another collision will occur in the same area. This is called clustering, and it’s a serious drawback to linear probing.

Worst case insertion, deletion, and lookup times have devolved to O(n), as the next available slot could potentially have been the last slot in the table.

![Hash Table](/../master/images/hashtable5.png?raw=true "hash table ex")

In the separate chaining model, the hash table is actually an array of pointers to linked lists.

When a collision occurs, the key can be inserted in constant time at the head of the appropriate linked list.

What happens now when we search for banana in the hash table? We must traverse the entire linked list at index 1. The worst case lookup time for a hash table that uses separate chaining (assuming a uniform hash function) is therefore O(n/m), where m is the size of the hash table.

Since m is a constant, O(n/m) is theoretically equivalent to O(n). In the real world, however, O(n/m) is a big improvement over O(n)!

A good hash function will maximize this real world improvement.

![Hash Table](/../master/images/hashtable6.png?raw=true "hash table ex")

https://www.youtube.com/watch?v=shs0KM3wKv8
	
```python
class HashMap:
	def __init__(self):
		self.size = 6
		self.map = [None] * self.size
		
	def _get_hash(self, key):
		hash = 0
		for char in str(key):
			hash += ord(char)
		return hash % self.size
		
	def add(self, key, value):
		key_hash = self._get_hash(key)
		key_value = [key, value]
		
		if self.map[key_hash] is None:
			self.map[key_hash] = list([key_value])
			return True
		else:
			for pair in self.map[key_hash]:
				if pair[0] == key:
					pair[1] = value
					return True
			self.map[key_hash].append(key_value)
			return True
			
	def get(self, key):
		key_hash = self._get_hash(key)
		if self.map[key_hash] is not None:
			for pair in self.map[key_hash]:
				if pair[0] == key:
					return pair[1]
		return None
			
	def delete(self, key):
		key_hash = self._get_hash(key)
		
		if self.map[key_hash] is None:
			return False
		for i in range (0, len(self.map[key_hash])):
			if self.map[key_hash][i][0] == key:
				self.map[key_hash].pop(i)
				return True
		return False
			
	def print(self):
		print('---PHONEBOOK----')
		for item in self.map:
			if item is not None:
				print(str(item))
			
h = HashMap()
h.add('Bob', '567-8888')
h.add('Ming', '293-6753')
h.add('Ming', '333-8233')
h.add('Ankit', '293-8625')
h.add('Aditya', '852-6551')
h.add('Alicia', '632-4123')
h.add('Mike', '567-2188')
h.add('Aditya', '777-8888')
h.print()		
h.delete('Bob')
h.print()
print('Ming: ' + h.get('Ming'))

```

## 8. Trees
*  basic tree construction
a tree is a (possibly non linear data structure made up of nodes or vertices and edges without having any cycle. The tree with no nodes is calle dthe null or empty tree. A tree that is not empty consists of a root node and potentially many levels of additional nodes that form a hierachy

```python 
class BinaryTree():

    def __init__(self,rootid):
      self.left = None
      self.right = None
      self.rootid = rootid

    def getLeftChild(self):
        return self.left
    def getRightChild(self):
        return self.right
    def setNodeValue(self,value):
        self.rootid = value
    def getNodeValue(self):
        return self.rootid
```

* traversal and manipulation algorithms.
usage patterns can be divided into the three ways that we access the nodes of the tree. There are three commonly used patterns to visit all the nodes in a tree. The difference between these patterns is the order in which each node is visited. We call this visitation of the nodes a “traversal.” The three traversals we will look at are called preorder, inorder, and postorder.

preorder - In a preorder traversal, we visit the root node first, then recursively do a preorder traversal of the left subtree, followed by a recursive preorder traversal of the right subtree.

```python 
def preorder(tree):
    if tree:
        print(tree.getNodeVal())
        preorder(tree.getLeftChild())
        preorder(tree.getRightChild())
```

inorder - In an inorder traversal, we recursively do an inorder traversal on the left subtree, visit the root node, and finally do a recursive inorder traversal of the right subtree.
```python
def inorder(tree):
  if tree != None:
      inorder(tree.getLeftChild())
      print(tree.getRootVal())
      inorder(tree.getRightChild())
```

postorder- In a postorder traversal, we recursively do a postorder traversal of the left subtree and the right subtree followed by a visit to the root node.

```python
def postorder(tree):
    if tree != None:
        postorder(tree.getLeftChild())
        postorder(tree.getRightChild())
        print(tree.getRootVal())
```

* Familiarize yourself with binary trees, n-ary trees, and trie-trees

binary trees = each node can have at most 2 children
https://www.youtube.com/watch?v=H5JubkIy_p8
![Binary Tree](/../master/images/binarytree.png?raw=true "binary tree")
	strict/Proper Binary tree --> each node can have either 2 or 0 children
	complete binary tree --> all levels except possibly the last are completely filled and all nodes are as left as possible

n-ary trees = trees to allow each parent to store references to any number of children. Such trees are called N-ary trees, and we can use them to represent the tree structure of a file system, where every node is either a file, or a folder (that can include other files and folders).
Here is an example of an N-ary tree representing a directory tree (with folder names in pink and file names in white)

trie-trees =  trees often used to store characters(great for word validation problems)
![Trie-trees](/../master/images/trie-trees1.png?raw=true "trie-trees ex")
![Trie-trees](/../master/images/trie-trees2.png?raw=true "trie-trees ex")
![Trie-trees](/../master/images/trie-trees3.png?raw=true "trie-trees ex")


* Be familiar with at least one type of balanced binary tree, whether it's a red/black tree, a splay tree or an AVL tree, and know how it's implemented

balanced binary trees: guaranteed hight of )(log n) for n items

red/black tree = 
a node is either red or blck
the root and leaves(nil) are black
if a node us red, then its children are black
all paths from a note to its NIL descendants contain the smae number of black nodes

![red-black tree](/../master/images/redblacktree1.png?raw=true "red black tree ex")

3 main operations are= 	search, time complexity = O(log n)  
			insert(require rotation), time complexity = O(log n)
			remove(require rotation) time complexity = O(log n)
space complexity = O(n)

![red-black ex](/../master/images/redblackex1.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex2.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex3.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex4.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex5.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex6.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex7.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex8.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex9.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex10.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex11.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex12.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex13.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex14.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex15.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex16.png?raw=true "red black ex")
![red-black ex](/../master/images/redblackex17.png?raw=true "red black ex")

```python
class RedBlackTree(object):
	  def __init__(self):
	    self._tree = None

	  def Insert(self, n):
	    if self._tree == None:
	      self._tree = RedBlackTreeNode(n)
	      self._tree.SetColor("Black")
	    else:
	      self._tree = self._tree.Insert(n)

	  def Print(self):
	    if self._tree == None:
	      print "Empty"
	    else:
	      self._tree.Print(1)


	class RedBlackTreeNode(object):
	  def __init__(self, value):
	    self._left = None
	    self._right = None
	    self._value = value
	    self.SetColor("Red")
	    self._parent = None

	  def GetParent(self):
	    return self._parent

	  def SetParent(self, parent):
	      self._parent = parent

	  def GetColor(self):
	    return self._color

	  def SetColor(self, color):
	      self._color = color

	  def GetLeft(self):
	      return self._left

	  def SetLeft(self, left):
	      self._left = left

	  def GetRight(self):
	      return self._right

	  def SetRight(self, right):
	      self._right = right

	  def GetGrandParent(self):
	    if self.GetParent() != None:
	      return self.GetParent().GetParent()
	    else:
	        return None

	  def GetUncle(self):
	    grand = self.GetGrandParent()
	    if grand is not None:
	      if grand.GetLeft() == self.GetParent():
	        return grand.GetRight()
	      else:
	        return grand.GetLeft()
	    else:
	      return None

	  def Rebalance(self):     
	      # WP case 1: tree root
	      if self.GetParent() is None:
	        self.SetColor("Black")
	        return self
	      # WP case 2: The parent of the target node is BLACK,
	      #   so the tree is in fine balance shape; just return the
	      #   tree root
	      if self.GetParent().GetColor() == "Black":
	          return self.GetRoot()
	      # From here on, we know that parent is Red.
	      # WP Case 3:  self, parent, and uncle are all red.
	      if self.GetUncle() is not None and self.GetUncle().GetColor() == "Red":
	          self.GetUncle().SetColor("Black")
	          self.GetParent().SetColor("Black")
	          self.GetGrandParent().SetColor("Red")
	          return self.GetGrandParent().Rebalance()
	      # By now, we know that self and parent are red; and the uncle is black.
	      # We also know that the grandparent is not None, because if it were, the
	      # parent would be root, which must be black. So this means that we 
	      # need to do a pivot on the parent
	      return self.PivotAndRebalance()

	  def GetRoot(self):
	    if self.GetParent() is None:
	      return self
	    else:
	      return self.GetParent().GetRoot()


	  def PivotAndRebalance(self):
	    # First, distinguish between the case where where my parent is
	    # a left child or a right child.
	    if self.GetGrandParent().GetLeft() == self.GetParent():
	      if self.GetParent().GetRight() == self:
	        # WP case 4: I'm the right child of my parent, and my parent is the
	        # left child of my grandparent. Pivot right around me.
	        return self.PivotLeft(False)
	      else:
	        # WP case 5
	        # If I'm the left child, and my parent is the left child, then
	        # pivot right around my parent.
	        return self.GetParent().PivotRight(True)
	    else: # My parent is the right child.
	      if self.GetParent().GetLeft() == self:
	        # WP case 4, reverse.
	        return self.PivotRight(False)
	      else:
	        # WY case 5 reverse
	        return self.GetParent().PivotLeft(True)


	  def PivotRight(self, recolor):
	      # Hurrah, I'm going to be the new root of the subtree!
	      left = self.GetLeft()
	      right = self.GetRight()
	      parent = self.GetParent()
	      grand = self.GetGrandParent()
	      # move my right child to be the left of my soon-to-be former parent.
	      parent.SetLeft(right)
	      if right is not None:
	          right.SetParent(parent)
	      # Move up, and make my old parent my right child.
	      self.SetParent(grand)
	      if grand is not None:
	        if  grand.GetRight(parent)  == parent:
	          grand.SetRight(self)
	        else:
	          grand.SetLeft(self)
	      self.SetRight(parent)
	      parent.SetParent(self)
	      if recolor is True:
	          parent.SetColor("Red")
	          self.SetColor("Black")
	          return self.GetRoot()
	      else:
	        # rebalance from the new position of my former parent.
	        return parent.Rebalance()

	  def PivotLeft(self, recolor):
	      # Hurrah, I'm going to be the new root of the subtree!
	      left = self.GetLeft()
	      right = self.GetRight()
	      parent = self.GetParent()
	      grand = self.GetGrandParent()
	      # move my left child to be the right of my soon-to-be former parent.
	      parent.SetRight(left)
	      if left is not None:
	          left.SetParent(parent)
	      # Move up, and make my old parent my right child.
	      self.SetParent(grand)
	      if grand is not None:
	        if  grand.GetRight() == parent:
	          grand.SetRight(self)
	        else:
	          grand.SetLeft(self)
	      self.SetLeft(parent)
	      parent.SetParent(self)
	      if recolor is True:
	        parent.SetColor("Red")
	        self.SetColor("Black")
	        return self.GetRoot()
	      else:
	        # rebalance from the position of my former parent.
	        return parent.Rebalance()


	  def Insert(self, value):
	    if self._value > value:
	      if self.GetLeft() is None:
	        self.SetLeft(RedBlackTreeNode(value))
	        self.GetLeft().SetParent(self)
	        return self.GetLeft().Rebalance()
	      else:
	        return self.GetLeft().Insert(value)
	    else:
	      if self.GetRight() is None:
	        self.SetRight(RedBlackTreeNode(value))
	        self.GetRight().SetParent(self)
	        return self.GetRight().Rebalance()        
	      else:
	        return self.GetRight().Insert(value)

	  def Print(self, indent):
	    for i in range(indent):
	      print "  ",
	    print "%s (%s)" % (self._value, self.GetColor())
	    if self.GetLeft() is None:
	      for i in range(indent+1):
	        print "  ",
	      print "None(Black)"
	    else:
	      self.GetLeft().Print(indent+1)
	    if self.GetRight() is None:
	      for i in range(indent+1):
	        print "  ",
	      print "None(Black)"
	    else:
	      self.GetRight().Print(indent+1)
	      
# For a quick example of creating and populating, here’s code to generatethe red-black tree used an an example above.

	b = RedBlackTree()
	for i in range(10):
	    b.Insert(i)
	b.Print()
```

splay tree = A splay tree is a self-adjusting binary search tree with the additional property that recently accessed elements are quick to access again. It performs basic operations such as insertion, look-up and removal in O(log n) amortized time.


Insert: O(log n)	Search: O(log n)
Delete: O(log n)	Algorithm: Average

splay tree example 
https://www.youtube.com/watch?v=nKZWL9hbcI4

insert 10
![splay tree ex](/../master/images/splaytree1.png?raw=true "splay tree ex")

insert 20
![splay tree ex](/../master/images/splaytree2.png?raw=true "splay tree ex")

zig left
![splay tree ex](/../master/images/splaytree3.png?raw=true "splay tree ex")

insert 30
![splay tree ex](/../master/images/splaytree4.png?raw=true "splay tree ex")

zig left
![splay tree ex](/../master/images/splaytree5.png?raw=true "splay tree ex")

insert 40
![splay tree ex](/../master/images/splaytree6.png?raw=true "splay tree ex")

 zig left
![splay tree ex](/../master/images/splaytree7.png?raw=true "splay tree ex")
 
 insert 25 
 ![splay tree ex](/../master/images/splaytree8.png?raw=true "splay tree ex")
 
 zig-zag right 
 ![splay tree ex](/../master/images/splaytree9.png?raw=true "splay tree ex")

 zig right 
  ![splay tree ex](/../master/images/splaytree10.png?raw=true "splay tree ex")

 insert 15 
 ![splay tree ex](/../master/images/splaytree11.png?raw=true "splay tree ex")
 
 zig-zag right 
 ![splay tree ex](/../master/images/splaytree12.png?raw=true "splay tree ex")
 
 zig right 
 ![splay tree ex](/../master/images/splaytree13.png?raw=true "splay tree ex")
 
 insert 120
 ![splay tree ex](/../master/images/splaytree14.png?raw=true "splay tree ex")
 
 zig-zag left
 ![splay tree ex](/../master/images/splaytree15.png?raw=true "splay tree ex")
 
 zig left 
 ![splay tree ex](/../master/images/splaytree16.png?raw=true "splay tree ex")
 
 find 25 
 ![splay tree ex](/../master/images/splaytree17.png?raw=true "splay tree ex")

 zig-zag left
 ![splay tree ex](/../master/images/splaytree18.png?raw=true "splay tree ex")
 
 zig right 
 ![splay tree ex](/../master/images/splaytree19.png?raw=true "splay tree ex")
 
 delete 15 
 ![splay tree ex](/../master/images/splaytree20.png?raw=true "splay tree ex")
 
 zig right
 ![splay tree ex](/../master/images/splaytree21.png?raw=true "splay tree ex")
 
 remove root 
 ![splay tree ex](/../master/images/splaytree22.png?raw=true "splay tree ex")
 
 largest element in left tree to root
 ![splay tree ex](/../master/images/splaytree23.png?raw=true "splay tree ex")
 
```python 
# splay tree
class Node:
    def __init__(self, key):
        self.key = key
        self.left = self.right = None

    def equals(self, node):
        return self.key == node.key

class SplayTree:
    def __init__(self):
        self.root = None
        self.header = Node(None) #For splay()

    def insert(self, key):
        if (self.root == None):
            self.root = Node(key)
            return

        self.splay(key)
        if self.root.key == key:
            # If the key is already there in the tree, don't do anything.
            return

        n = Node(key)
        if key < self.root.key:
            n.left = self.root.left
            n.right = self.root
            self.root.left = None
        else:
            n.right = self.root.right
            n.left = self.root
            self.root.right = None
        self.root = n

    def remove(self, key):
        self.splay(key)
        if key != self.root.key:
            raise 'key not found in tree'

        # Now delete the root.
        if self.root.left== None:
            self.root = self.root.right
        else:
            x = self.root.right
            self.root = self.root.left
            self.splay(key)
            self.root.right = x

    def findMin(self):
        if self.root == None:
            return None
        x = self.root
        while x.left != None:
            x = x.left
        self.splay(x.key)
        return x.key

    def findMax(self):
        if self.root == None:
            return None
        x = self.root
        while (x.right != None):
            x = x.right
        self.splay(x.key)
        return x.key

    def find(self, key):
        if self.root == None:
            return None
        self.splay(key)
        if self.root.key != key:
            return None
        return self.root.key

    def isEmpty(self):
        return self.root == None
    
    def splay(self, key):
        l = r = self.header
        t = self.root
        self.header.left = self.header.right = None
        while True:
            if key < t.key:
                if t.left == None:
                    break
                if key < t.left.key:
                    y = t.left
                    t.left = y.right
                    y.right = t
                    t = y
                    if t.left == None:
                        break
                r.left = t
                r = t
                t = t.left
            elif key > t.key:
                if t.right == None:
                    break
                if key > t.right.key:
                    y = t.right
                    t.right = y.left
                    y.left = t
                    t = y
                    if t.right == None:
                        break
                l.right = t
                l = t
                t = t.right
            else:
                break
        l.right = t.left
        r.left = t.right
        t.left = self.header.right
        t.right = self.header.left
        self.root = t
```

AVL tree = AVL tree is a self-balancing binary search tree. It was the first such data structure to be invented. In an AVL tree, the heights of the two child subtrees of any node differ by at most one; if at any time they differ by more than one, rebalancing is done to restore this property.

Search: O(log n)	Algorithm: Average
Insert: O(log n)	Delete: O(log n)

To balance itself, an AVL tree may perform the following four kinds of rotations −

Left rotation
Right rotation
Left-Right rotation
Right-Left rotation
The first two rotations are single rotations and the next two rotations are double rotations. To have an unbalanced tree, we at least need a tree of height 2. With this simple tree, let's understand them one by one.

Left Rotation
If a tree becomes unbalanced, when a node is inserted into the right subtree of the right subtree, then we perform a single left rotation −
![AVL left rotate](/../master/images/avlleftrotation.png?raw=true "AVL left rotation")

Right Rotation
AVL tree may become unbalanced, if a node is inserted in the left subtree of the left subtree. The tree then needs a right rotation.
![AVL right rotate](/../master/images/avlrightrotation.png?raw=true "AVL right rotation")

Left-Right Rotation
Double rotations are slightly complex version of already explained versions of rotations. To understand them better, we should take note of each action performed while rotation. Let's first check how to perform Left-Right rotation. A left-right rotation is a combination of left rotation followed by right rotation.
![AVL left-right rotate](/../master/images/avlleftrightrotation.png?raw=true "AVL left-right rotation")

Right-Left Rotation
The second type of double rotation is Right-Left Rotation. It is a combination of right rotation followed by left rotation.
![AVL right-left rotate](/../master/images/avltrightleftrotation.png?raw=true "AVL right-left rotation")

example of AVL insert the following: 3-4-8-6-7-5-2-1
![AVL tree ex](/../master/images/avlex1.png?raw=true "AVL tree ex")
![AVL tree ex](/../master/images/avlex2.png?raw=true "AVL tree ex")
![AVL tree ex](/../master/images/avlex3.png?raw=true "AVL tree ex")
![AVL tree ex](/../master/images/avlex4.png?raw=true "AVL tree ex")
![AVL tree ex](/../master/images/avlex5.png?raw=true "AVL tree ex")
![AVL tree ex](/../master/images/avlex6.png?raw=true "AVL tree ex")
![AVL tree ex](/../master/images/avlex7.png?raw=true "AVL tree ex")
![AVL tree ex](/../master/images/avlex8.png?raw=true "AVL tree ex")
![AVL tree ex](/../master/images/avlex9.png?raw=true "AVL tree ex")
![AVL tree ex](/../master/images/avlex10.png?raw=true "AVL tree ex")
![AVL tree ex](/../master/images/avlex11.png?raw=true "AVL tree ex")
![AVL tree ex](/../master/images/avlex12.png?raw=true "AVL tree ex")
![AVL tree ex](/../master/images/avlex13.png?raw=true "AVL tree ex")
![AVL tree ex](/../master/images/avlex14.png?raw=true "AVL tree ex")

AVL deletion = https://www.youtube.com/watch?v=7w9Ge_IjSOw

```python 
#import random, math

outputdebug = False 

def debug(msg):
    if outputdebug:
        print msg

class Node():
    def __init__(self, key):
        self.key = key
        self.left = None 
        self.right = None 

class AVLTree():
    def __init__(self, *args):
        self.node = None 
        self.height = -1  
        self.balance = 0; 
        
        if len(args) == 1: 
            for i in args[0]: 
                self.insert(i)
                
    def height(self):
        if self.node: 
            return self.node.height 
        else: 
            return 0 
    
    def is_leaf(self):
        return (self.height == 0) 
    
    def insert(self, key):
        tree = self.node
        
        newnode = Node(key)
        
        if tree == None:
            self.node = newnode 
            self.node.left = AVLTree() 
            self.node.right = AVLTree()
            debug("Inserted key [" + str(key) + "]")
        
        elif key < tree.key: 
            self.node.left.insert(key)
            
        elif key > tree.key: 
            self.node.right.insert(key)
        
        else: 
            debug("Key [" + str(key) + "] already in tree.")
            
        self.rebalance() 
        
    def rebalance(self):
        ''' 
        Rebalance a particular (sub)tree
        ''' 
        # key inserted. Let's check if we're balanced
        self.update_heights(False)
        self.update_balances(False)
        while self.balance < -1 or self.balance > 1: 
            if self.balance > 1:
                if self.node.left.balance < 0:  
                    self.node.left.lrotate() # we're in case II
                    self.update_heights()
                    self.update_balances()
                self.rrotate()
                self.update_heights()
                self.update_balances()
                
            if self.balance < -1:
                if self.node.right.balance > 0:  
                    self.node.right.rrotate() # we're in case III
                    self.update_heights()
                    self.update_balances()
                self.lrotate()
                self.update_heights()
                self.update_balances()


            
    def rrotate(self):
        # Rotate left pivoting on self
        debug ('Rotating ' + str(self.node.key) + ' right') 
        A = self.node 
        B = self.node.left.node 
        T = B.right.node 
        
        self.node = B 
        B.right.node = A 
        A.left.node = T 

    
    def lrotate(self):
        # Rotate left pivoting on self
        debug ('Rotating ' + str(self.node.key) + ' left') 
        A = self.node 
        B = self.node.right.node 
        T = B.left.node 
        
        self.node = B 
        B.left.node = A 
        A.right.node = T 
        
            
    def update_heights(self, recurse=True):
        if not self.node == None: 
            if recurse: 
                if self.node.left != None: 
                    self.node.left.update_heights()
                if self.node.right != None:
                    self.node.right.update_heights()
            
            self.height = max(self.node.left.height,
                              self.node.right.height) + 1 
        else: 
            self.height = -1 
            
    def update_balances(self, recurse=True):
        if not self.node == None: 
            if recurse: 
                if self.node.left != None: 
                    self.node.left.update_balances()
                if self.node.right != None:
                    self.node.right.update_balances()

            self.balance = self.node.left.height - self.node.right.height 
        else: 
            self.balance = 0 

    def delete(self, key):
        # debug("Trying to delete at node: " + str(self.node.key))
        if self.node != None: 
            if self.node.key == key: 
                debug("Deleting ... " + str(key))  
                if self.node.left.node == None and self.node.right.node == None:
                    self.node = None # leaves can be killed at will 
                # if only one subtree, take that 
                elif self.node.left.node == None: 
                    self.node = self.node.right.node
                elif self.node.right.node == None: 
                    self.node = self.node.left.node
                
                # worst-case: both children present. Find logical successor
                else:  
                    replacement = self.logical_successor(self.node)
                    if replacement != None: # sanity check 
                        debug("Found replacement for " + str(key) + " -> " + str(replacement.key))  
                        self.node.key = replacement.key 
                        
                        # replaced. Now delete the key from right child 
                        self.node.right.delete(replacement.key)
                    
                self.rebalance()
                return  
            elif key < self.node.key: 
                self.node.left.delete(key)  
            elif key > self.node.key: 
                self.node.right.delete(key)
                        
            self.rebalance()
        else: 
            return 

    def logical_predecessor(self, node):
        ''' 
        Find the biggest valued node in LEFT child
        ''' 
        node = node.left.node 
        if node != None: 
            while node.right != None:
                if node.right.node == None: 
                    return node 
                else: 
                    node = node.right.node  
        return node 
    
    def logical_successor(self, node):
        ''' 
        Find the smallese valued node in RIGHT child
        ''' 
        node = node.right.node  
        if node != None: # just a sanity check  
            
            while node.left != None:
                debug("LS: traversing: " + str(node.key))
                if node.left.node == None: 
                    return node 
                else: 
                    node = node.left.node  
        return node 

    def check_balanced(self):
        if self == None or self.node == None: 
            return True
        
        # We always need to make sure we are balanced 
        self.update_heights()
        self.update_balances()
        return ((abs(self.balance) < 2) and self.node.left.check_balanced() and self.node.right.check_balanced())  
        
    def inorder_traverse(self):
        if self.node == None:
            return [] 
        
        inlist = [] 
        l = self.node.left.inorder_traverse()
        for i in l: 
            inlist.append(i) 

        inlist.append(self.node.key)

        l = self.node.right.inorder_traverse()
        for i in l: 
            inlist.append(i) 
    
        return inlist 

    def display(self, level=0, pref=''):
        '''
        Display the whole tree. Uses recursive def.
        TODO: create a better display using breadth-first search
        '''        
        self.update_heights()  # Must update heights before balances 
        self.update_balances()
        if(self.node != None): 
            print '-' * level * 2, pref, self.node.key, "[" + str(self.height) + ":" + str(self.balance) + "]", 'L' if self.is_leaf() else ' '    
            if self.node.left != None: 
                self.node.left.display(level + 1, '<')
            if self.node.left != None:
                self.node.right.display(level + 1, '>')
        

# Usage example
if __name__ == "__main__": 
    a = AVLTree()
    print "----- Inserting -------"
    #inlist = [5, 2, 12, -4, 3, 21, 19, 25]
    inlist = [7, 5, 2, 6, 3, 4, 1, 8, 9, 0]
    for i in inlist: 
        a.insert(i)
         
    a.display()
    
    print "----- Deleting -------"
    a.delete(3)
    a.delete(4)
    # a.delete(5) 
    a.display()
    
    print 
    print "Input            :", inlist 
    print "deleting ...       ", 3
    print "deleting ...       ", 4
    print "Inorder traversal:", a.inorder_traverse() 
```

* Understand tree traversal algorithms: BFS and DFS, and know the difference between inorder, postorder and preorder.
Breadth First Traversal(level order) = referes to traversing the tree nodes in a level by level fashion

![Breadth First Traversal](/../master/images/breadthfirsttraversal.png?raw=true "Breadth First Traversal ex")
use BFS when you need to find the shortest path in a unweighted graph

https://www.youtube.com/watch?v=-uR7BSfNJko

```python
class Vertex:
	def __init__(self, n):
		self.name = n
		self.neighbors = list()
		
		self.distance = 9999
		self.color = 'black'
	
	def add_neighbor(self, v):
		if v not in self.neighbors:
			self.neighbors.append(v)
			self.neighbors.sort()

class Graph:
	vertices = {}
	
	def add_vertex(self, vertex):
		if isinstance(vertex, Vertex) and vertex.name not in self.vertices:
			self.vertices[vertex.name] = vertex
			return True
		else:
			return False
	
	def add_edge(self, u, v):
		if u in self.vertices and v in self.vertices:
			for key, value in self.vertices.items():
				if key == u:
					value.add_neighbor(v)
				if key == v:
					value.add_neighbor(u)
			return True
		else:
			return False
			
	def print_graph(self):
		for key in sorted(list(self.vertices.keys())):
			print(key + str(self.vertices[key].neighbors) + "  " + str(self.vertices[key].distance))
		
	def bfs(self, vert):
		q = list()
		vert.distance = 0
		vert.color = 'red'
		for v in vert.neighbors:
			self.vertices[v].distance = vert.distance + 1
			q.append(v)
		
		while len(q) > 0:
			u = q.pop(0)
			node_u = self.vertices[u]
			node_u.color = 'red'
			
			for v in node_u.neighbors:
				node_v = self.vertices[v]
				if node_v.color == 'black':
					q.append(v)
					if node_v.distance > node_u.distance + 1:
						node_v.distance = node_u.distance + 1
					
g = Graph()
a = Vertex('A')
g.add_vertex(a)
g.add_vertex(Vertex('B'))
for i in range(ord('A'), ord('K')):
	g.add_vertex(Vertex(chr(i)))

edges = ['AB', 'AE', 'BF', 'CG', 'DE', 'DH', 'EH', 'FG', 'FI', 'FJ', 'GJ', 'HI']
for edge in edges:
	g.add_edge(edge[:1], edge[1:])
	
g.bfs(a)
g.print_graph()
```

Depth first Traversal = is an algorithm for traversing or searching tree or graph data structures. One starts at the root (selecting some arbitrary node as the root in the case of a graph) and explores as far as possible along each branch before backtracking.

Preorder traversal - Root, left, Right
![Preorder Traversal](/../master/images/preordertraversal.png?raw=true "Preorder Traversal ex")
	
Inorder traversal - Left, Root, Right
![Inorder Traversal](/../master/images/inordertraversal.png?raw=true "Inorder Traversal ex")
	
Postorder Traversal - left, right, root
![Postorder Traversal](/../master/images/postordertraversal.png?raw=true "Postorder Traversal ex")

```python
# depth First Search

class Vertex:
	def __init__(self, n):
		self.name = n
		self.neighbors = list()
		
		self.discovery = 0
		self.finish = 0
		self.color = 'black'
	
	def add_neighbor(self, v):
		if v not in self.neighbors:
			self.neighbors.append(v)
			self.neighbors.sort()

class Graph:
	vertices = {}
	time = 0
	
	def add_vertex(self, vertex):
		if isinstance(vertex, Vertex) and vertex.name not in self.vertices:
			self.vertices[vertex.name] = vertex
			return True
		else:
			return False
	
	def add_edge(self, u, v):
		if u in self.vertices and v in self.vertices:
			for key, value in self.vertices.items():
				if key == u:
					value.add_neighbor(v)
				if key == v:
					value.add_neighbor(u)
			return True
		else:
			return False
			
	def print_graph(self):
		for key in sorted(list(self.vertices.keys())):
			print(key + str(self.vertices[key].neighbors) + "  " + str(self.vertices[key].discovery) + "/" + str(self.vertices[key].finish))

	def _dfs(self, vertex):
		global time
		vertex.color = 'red'
		vertex.discovery = time
		time += 1
		for v in vertex.neighbors:
			if self.vertices[v].color == 'black':
				self._dfs(self.vertices[v])
		vertex.color = 'blue'
		vertex.finish = time
		time += 1
		
	def dfs(self, vertex):
		global time
		time = 1
		self._dfs(vertex)
			
g = Graph()
# print(str(len(g.vertices)))
a = Vertex('A')
g.add_vertex(a)
g.add_vertex(Vertex('B'))
for i in range(ord('A'), ord('K')):
	g.add_vertex(Vertex(chr(i)))

edges = ['AB', 'AE', 'BF', 'CG', 'DE', 'DH', 'EH', 'FG', 'FI', 'FJ', 'GJ', 'HI']
for edge in edges:
	g.add_edge(edge[:1], edge[1:])
	
g.dfs(a)
g.print_graph()

```

binary search Tree:
orderd, or sorted, binary trees
nodes can have 2 subtrees
items to the left of a given node are smaller
items to the right of a given node are larger

 allow fast lookup, addition and removal of items, and can be used to implement either dynamic sets of items, or lookup tables that allow finding an item by its key (e.g., finding the phone number of a person by name).
 
 The major advantage of binary search trees over other data structures is that the related sorting algorithms and search algorithms such as in-order traversal can be very efficient; they are also easy to code.

Binary search trees are a fundamental data structure used to construct more abstract data structures such as sets, multisets, and associative arrays. Some of their disadvantages are as follows:

The shape of the binary search tree depends entirely on the order of insertions and deletions, and can become degenerate.
When inserting or searching for an element in a binary search tree, the key of each visited node has to be compared with the key of the element to be inserted or found.
The keys in the binary search tree may be long and the run time may increase.
After a long intermixed sequence of random insertion and deletion, the expected height of the tree approaches square root of the number of keys, √n, which grows much faster than log n.

```python
# Binary Search Tree in Python
class Node:
	def __init__(self, val):
		self.value = val
		self.leftChild = None
		self.rightChild = None
		
	def insert(self, data):
		if self.value == data:
			return False
			
		elif self.value > data:
			if self.leftChild:
				return self.leftChild.insert(data)
			else:
				self.leftChild = Node(data)
				return True

		else:
			if self.rightChild:
				return self.rightChild.insert(data)
			else:
				self.rightChild = Node(data)
				return True
				
	def find(self, data):
		if(self.value == data):
			return True
		elif self.value > data:
			if self.leftChild:
				return self.leftChild.find(data)
			else:
				return False
		else:
			if self.rightChild:
				return self.rightChild.find(data)
			else:
				return False

	def preorder(self):
		if self:
			print (str(self.value))
			if self.leftChild:
				self.leftChild.preorder()
			if self.rightChild:
				self.rightChild.preorder()

	def postorder(self):
		if self:
			if self.leftChild:
				self.leftChild.postorder()
			if self.rightChild:
				self.rightChild.postorder()
			print (str(self.value))

	def inorder(self):
		if self:
			if self.leftChild:
				self.leftChild.inorder()
			print (str(self.value))
			if self.rightChild:
				self.rightChild.inorder()

class Tree:
	def __init__(self):
		self.root = None

	def insert(self, data):
		if self.root:
			return self.root.insert(data)
		else:
			self.root = Node(data)
			return True

	def find(self, data):
		if self.root:
			return self.root.find(data)
		else:
			return False
	
	def remove(self, data):
		# empty tree
		if self.root is None:
			return False
			
		# data is in root node	
		elif self.root.value == data:
			if self.root.leftChild is None and self.root.rightChild is None:
				self.root = None
			elif self.root.leftChild and self.root.rightChild is None:
				self.root = self.root.leftChild
			elif self.root.leftChild is None and self.root.rightChild:
				self.root = self.root.rightChild
			elif self.root.leftChild and self.root.rightChild:
				delNodeParent = self.root
				delNode = self.root.rightChild
				while delNode.leftChild:
					delNodeParent = delNode
					delNode = delNode.leftChild
					
				self.root.value = delNode.value
				if delNode.rightChild:
					if delNodeParent.value > delNode.value:
						delNodeParent.leftChild = delNode.rightChild
					elif delNodeParent.value < delNode.value:
						delNodeParent.rightChild = delNode.rightChild
				else:
					if delNode.value < delNodeParent.value:
						delNodeParent.leftChild = None
					else:
						delNodeParent.rightChild = None
						
			return True
		
		parent = None
		node = self.root
		
		# find node to remove
		while node and node.value != data:
			parent = node
			if data < node.value:
				node = node.leftChild
			elif data > node.value:
				node = node.rightChild
		
		# case 1: data not found
		if node is None or node.value != data:
			return False
			
		# case 2: remove-node has no children
		elif node.leftChild is None and node.rightChild is None:
			if data < parent.value:
				parent.leftChild = None
			else:
				parent.rightChild = None
			return True
			
		# case 3: remove-node has left child only
		elif node.leftChild and node.rightChild is None:
			if data < parent.value:
				parent.leftChild = node.leftChild
			else:
				parent.rightChild = node.leftChild
			return True
			
		# case 4: remove-node has right child only
		elif node.leftChild is None and node.rightChild:
			if data < parent.value:
				parent.leftChild = node.rightChild
			else:
				parent.rightChild = node.rightChild
			return True
			
		# case 5: remove-node has left and right children
		else:
			delNodeParent = node
			delNode = node.rightChild
			while delNode.leftChild:
				delNodeParent = delNode
				delNode = delNode.leftChild
				
			node.value = delNode.value
			if delNode.rightChild:
				if delNodeParent.value > delNode.value:
					delNodeParent.leftChild = delNode.rightChild
				elif delNodeParent.value < delNode.value:
					delNodeParent.rightChild = delNode.rightChild
			else:
				if delNode.value < delNodeParent.value:
					delNodeParent.leftChild = None
				else:
					delNodeParent.rightChild = None

	def preorder(self):
		if self.root is not None:
			print("PreOrder")
			self.root.preorder()
        
	def postorder(self):
		if self.root is not None:
			print("PostOrder")
			self.root.postorder()
			
	def inorder(self):
		if self.root is not None:
			print("InOrder")
			self.root.inorder()

bst = Tree()
print(bst.insert(10))

bst.preorder()
#bst.postorder()
#bst.inorder()
print(bst.remove(10))
bst.preorder()
			
```

cartesian Tree:
a type of data structure used in computer science for encoding a sequence of numbers.

find the pivot, which is always going to the smallest number, and then divide the list up into a sublist, where the first index is the current node, the second is the left child, and the third is the right. If left and right are empty, then the singleton list denotes a leaf.

ex.  [9, 3, 7, 1, 8, 12, 10, 20, 15, 18, 5]

The first thing we need to do is find the pivot. As mentioned above, it’s always the smallest number. In this case, 1 is the pivot, so it becomes the root node, since all other elements will become leaves, and this is the first node.

![Cartesian Tree](/../master/images/cartesiantree1.png?raw=true "Cartesian tree ex")

Now, for each new side, we need to do the same thing. So, on the left hand, 3 is the smallest, and on the right, 5 is the smallest.

![Cartesian Tree](/../master/images/cartesiantree2.png?raw=true "Cartesian tree ex")

Visually, this is a bit odd because of spacing, but we can see all the numbers on the right branch (5) are to the left of 5 – it will look better once we finish it.

Next up is left, where there is nothing to be done, since each child of 3 is a leaf, and right, where 8 is smallest.

![Cartesian Tree](/../master/images/cartesiantree3.png?raw=true "Cartesian tree ex")

And again, 10 is now the smallest.

![Cartesian Tree](/../master/images/cartesiantree4.png?raw=true "Cartesian tree ex")

And now, almost done, the right side is left; 15 is the smallest here.

![Cartesian Tree](/../master/images/cartesiantree4.png?raw=true "Cartesian tree ex")

sorting of a cartesian tree
algo = 
Construct a Cartesian tree for the input sequence
Initialize a priority queue, initially containing only the tree root
While the priority queue is non-empty:
	Find and remove the minimum value x in the priority queue
	Add x to the output sequence
	Add the Cartesian tree children of x to the priority queue

ex. 
![Cartesian Tree Sorting](/../master/images/cartesianSorting.JPG?raw=true "Cartesian tree sorting ex")

```C++
// A C++ program to implement Cartesian Tree sort
// Note that in this program we will build a min-heap
// Cartesian Tree and not max-heap.
#include<bits/stdc++.h>
using namespace std;
 
/* A binary tree node has data, pointer to left child
   and a pointer to right child */
struct Node
{
    int data;
    Node *left, *right;
};
 
// Creating a shortcut for int, Node* pair type
typedef pair<int, Node*> iNPair;
 
// This function sorts by pushing and popping the
// Cartesian Tree nodes in a pre-order like fashion
void pQBasedTraversal(Node* root)
{
    // We will use a priority queue to sort the
    // partially-sorted data efficiently.
    // Unlike Heap, Cartesian tree makes use of
    // the fact that the data is partially sorted
    priority_queue <iNPair, vector<iNPair>, greater<iNPair>> pQueue;
    pQueue.push (make_pair (root->data,root));
 
    // Resembles a pre-order traverse as first data
    // is printed then the left and then right child.
    while (! pQueue.empty())
    {
        iNPair popped_pair = pQueue.top();
        printf("%d ",popped_pair.first);
 
        pQueue.pop();
 
        if (popped_pair.second->left != NULL)
            pQueue.push (make_pair(popped_pair.second->left->data,
                                   popped_pair.second->left));
 
        if (popped_pair.second->right != NULL)
             pQueue.push (make_pair(popped_pair.second->right->data,
                                    popped_pair.second->right));
    }
 
    return;
}
 
 
Node *buildCartesianTreeUtil(int root, int arr[],
           int parent[], int leftchild[], int rightchild[])
{
    if (root == -1)
        return NULL;
 
    Node *temp = new Node;
 
    temp->data = arr[root];
    temp->left = buildCartesianTreeUtil(leftchild[root],
                  arr, parent, leftchild, rightchild);
 
    temp->right = buildCartesianTreeUtil(rightchild[root],
                  arr, parent, leftchild, rightchild);
 
    return temp ;
}
 
// A function to create the Cartesian Tree in O(N) time
Node *buildCartesianTree(int arr[], int n)
{
    // Arrays to hold the index of parent, left-child,
    // right-child of each number in the input array
    int parent[n],leftchild[n],rightchild[n];
 
    // Initialize all array values as -1
    memset(parent, -1, sizeof(parent));
    memset(leftchild, -1, sizeof(leftchild));
    memset(rightchild, -1, sizeof(rightchild));
 
    // 'root' and 'last' stores the index of the root and the
    // last processed of the Cartesian Tree.
    // Initially we take root of the Cartesian Tree as the
    // first element of the input array. This can change
    // according to the algorithm
    int root = 0, last;
 
    // Starting from the second element of the input array
    // to the last on scan across the elements, adding them
    // one at a time.
    for (int i=1; i<=n-1; i++)
    {
        last = i-1;
        rightchild[i] = -1;
 
        // Scan upward from the node's parent up to
        // the root of the tree until a node is found
        // whose value is smaller than the current one
        // This is the same as Step 2 mentioned in the
        // algorithm
        while (arr[last] >= arr[i] && last != root)
            last = parent[last];
 
        // arr[i] is the smallest element yet; make it
        // new root
        if (arr[last] >= arr[i])
        {
            parent[root] = i;
            leftchild[i] = root;
            root = i;
        }
 
        // Just insert it
        else if (rightchild[last] == -1)
        {
            rightchild[last] = i;
            parent[i] = last;
            leftchild[i] = -1;
        }
 
        // Reconfigure links
        else
        {
            parent[rightchild[last]] = i;
            leftchild[i] = rightchild[last];
            rightchild[last]= i;
            parent[i] = last;
        }
 
    }
 
    // Since the root of the Cartesian Tree has no
    // parent, so we assign it -1
    parent[root] = -1;
 
    return (buildCartesianTreeUtil (root, arr, parent,
                                    leftchild, rightchild));
}
 
// Sorts an input array
int printSortedArr(int arr[], int n)
{
    // Build a cartesian tree
    Node *root = buildCartesianTree(arr, n);
 
    printf("The sorted array is-\n");
 
    // Do pr-order traversal and insert
    // in priority queue
    pQBasedTraversal(root);
}
 
/* Driver program to test above functions */
int main()
{
    /*  Given input array- {5,10,40,30,28},
        it's corresponding unique Cartesian Tree
        is-
 
        5
          \
          10
            \
             28
            /
          30
         /
        40
    */
 
    int arr[] = {5, 10, 40, 30, 28};
    int n = sizeof(arr)/sizeof(arr[0]);
 
    printSortedArr(arr, n);
 
    return(0);
}

```

B-tree:
Not to be coinfussed with a Binary tree a allows searches, sequential access, insertions, and deletions in logarithmic time. The B-tree is a generalization of a binary search tree in that a node can have more than two children (Comer 1979, p. 123). Unlike self-balancing binary search trees, the B-tree is optimized for systems that read and write large blocks of data. B-trees are a good example of a data structure for external memory. It is commonly used in databases and filesystems.

Time complexity in big O notation
Algorithm	Average		Worst Case
Space		O(n)		O(n)
Search		O(log n)	O(log n)
Insert		O(log n)	O(log n)
Delete		O(log n)	O(log n)

advantages: 
keeps keys in sorted order for sequential traversing
uses a hierarchical index to minimize the number of disk reads
uses partially full blocks to speed insertions and deletions
keeps the index balanced with a recursive algorithm
In addition, a B-tree minimizes waste by making sure the interior nodes are at least half full. A B-tree can handle an arbitrary number of insertions and deletions.

disadvantages: 
maximum key length cannot be changed without completely rebuilding the database. This led to many database systems truncating full human names to 70 characters.

https://www.youtube.com/watch?v=coRJrcIYbF4

ex= [2, 4, 5, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30 , 32, 34, 36, 7, 9, 11, , 13]

add 2
![B Tree ex](/../master/images/btree1.png?raw=true "B tree ex")

add 4
![B Tree ex](/../master/images/btree2.png?raw=true "B tree ex")

add 5 
![B Tree ex](/../master/images/btree3.png?raw=true "B tree ex")

add 6 
![B Tree ex](/../master/images/btree4.png?raw=true "B tree ex")

add 8 
![B Tree ex](/../master/images/btree5.png?raw=true "B tree ex")

add 10 
![B Tree ex](/../master/images/btree6.png?raw=true "B tree ex")

add 12 
![B Tree ex](/../master/images/btree7.png?raw=true "B tree ex")

add 14 
![B Tree ex](/../master/images/btree8.png?raw=true "B tree ex")

add 16
![B Tree ex](/../master/images/btree9.png?raw=true "B tree ex")

add 18
![B Tree ex](/../master/images/btree10.png?raw=true "B tree ex")

add 20
![B Tree ex](/../master/images/btree11.png?raw=true "B tree ex")

add 22 
![B Tree ex](/../master/images/btree12.png?raw=true "B tree ex")

add 24
![B Tree ex](/../master/images/btree13.png?raw=true "B tree ex")

add 26 
![B Tree ex](/../master/images/btree14.png?raw=true "B tree ex")

add 28 
![B Tree ex](/../master/images/btree15.png?raw=true "B tree ex")

add 30 
![B Tree ex](/../master/images/btree16.png?raw=true "B tree ex")

add 32
![B Tree ex](/../master/images/btree17.png?raw=true "B tree ex")

add 34
![B Tree ex](/../master/images/btree18.png?raw=true "B tree ex")

add 36 
![B Tree ex](/../master/images/btree19.png?raw=true "B tree ex")

add 7 
![B Tree ex](/../master/images/btree20.png?raw=true "B tree ex")

add 9 
![B Tree ex](/../master/images/btree21.png?raw=true "B tree ex")

add 11 
![B Tree ex](/../master/images/btree22.png?raw=true "B tree ex")

add 13 
![B Tree ex](/../master/images/btree23.png?raw=true "B tree ex")

delete 14 
![B Tree ex](/../master/images/btree24.png?raw=true "B tree ex")

delete 10
![B Tree ex](/../master/images/btree25.png?raw=true "B tree ex")

delete 18 
![B Tree ex](/../master/images/btree26.png?raw=true "B tree ex")

delete 16
![B Tree ex](/../master/images/btree27.png?raw=true "B tree ex")

delete 24
![B Tree ex](/../master/images/btree28.png?raw=true "B tree ex")

delete 6
![B Tree ex](/../master/images/btree29.png?raw=true "B tree ex")

```python
import bisect
import itertools
import operator

class _BNode(object):
    __slots__ = ["tree", "contents", "children"]

    def __init__(self, tree, contents=None, children=None):
        self.tree = tree
        self.contents = contents or []
        self.children = children or []
        if self.children:
            assert len(self.contents) + 1 == len(self.children), \
                    "one more child than data item required"

    def __repr__(self):
        name = getattr(self, "children", 0) and "Branch" or "Leaf"
        return "<%s %s>" % (name, ", ".join(map(str, self.contents)))

    def lateral(self, parent, parent_index, dest, dest_index):
        if parent_index > dest_index:
            dest.contents.append(parent.contents[dest_index])
            parent.contents[dest_index] = self.contents.pop(0)
            if self.children:
                dest.children.append(self.children.pop(0))
        else:
            dest.contents.insert(0, parent.contents[parent_index])
            parent.contents[parent_index] = self.contents.pop()
            if self.children:
                dest.children.insert(0, self.children.pop())

    def shrink(self, ancestors):
        parent = None

        if ancestors:
            parent, parent_index = ancestors.pop()
            # try to lend to the left neighboring sibling
            if parent_index:
                left_sib = parent.children[parent_index - 1]
                if len(left_sib.contents) < self.tree.order:
                    self.lateral(
                            parent, parent_index, left_sib, parent_index - 1)
                    return

            # try the right neighbor
            if parent_index + 1 < len(parent.children):
                right_sib = parent.children[parent_index + 1]
                if len(right_sib.contents) < self.tree.order:
                    self.lateral(
                            parent, parent_index, right_sib, parent_index + 1)
                    return

        center = len(self.contents) // 2
        sibling, push = self.split()

        if not parent:
            parent, parent_index = self.tree.BRANCH(
                    self.tree, children=[self]), 0
            self.tree._root = parent

        # pass the median up to the parent
        parent.contents.insert(parent_index, push)
        parent.children.insert(parent_index + 1, sibling)
        if len(parent.contents) > parent.tree.order:
            parent.shrink(ancestors)

    def grow(self, ancestors):
        parent, parent_index = ancestors.pop()

        minimum = self.tree.order // 2
        left_sib = right_sib = None

        # try to borrow from the right sibling
        if parent_index + 1 < len(parent.children):
            right_sib = parent.children[parent_index + 1]
            if len(right_sib.contents) > minimum:
                right_sib.lateral(parent, parent_index + 1, self, parent_index)
                return

        # try to borrow from the left sibling
        if parent_index:
            left_sib = parent.children[parent_index - 1]
            if len(left_sib.contents) > minimum:
                left_sib.lateral(parent, parent_index - 1, self, parent_index)
                return

        # consolidate with a sibling - try left first
        if left_sib:
            left_sib.contents.append(parent.contents[parent_index - 1])
            left_sib.contents.extend(self.contents)
            if self.children:
                left_sib.children.extend(self.children)
            parent.contents.pop(parent_index - 1)
            parent.children.pop(parent_index)
        else:
            self.contents.append(parent.contents[parent_index])
            self.contents.extend(right_sib.contents)
            if self.children:
                self.children.extend(right_sib.children)
            parent.contents.pop(parent_index)
            parent.children.pop(parent_index + 1)

        if len(parent.contents) < minimum:
            if ancestors:
                # parent is not the root
                parent.grow(ancestors)
            elif not parent.contents:
                # parent is root, and its now empty
                self.tree._root = left_sib or self

    def split(self):
        center = len(self.contents) // 2
        median = self.contents[center]
        sibling = type(self)(
                self.tree,
                self.contents[center + 1:],
                self.children[center + 1:])
        self.contents = self.contents[:center]
        self.children = self.children[:center + 1]
        return sibling, median

    def insert(self, index, item, ancestors):
        self.contents.insert(index, item)
        if len(self.contents) > self.tree.order:
            self.shrink(ancestors)

    def remove(self, index, ancestors):
        minimum = self.tree.order // 2

        if self.children:
            # try promoting from the right subtree first,
            # but only if it won't have to resize
            additional_ancestors = [(self, index + 1)]
            descendent = self.children[index + 1]
            while descendent.children:
                additional_ancestors.append((descendent, 0))
                descendent = descendent.children[0]
            if len(descendent.contents) > minimum:
                ancestors.extend(additional_ancestors)
                self.contents[index] = descendent.contents[0]
                descendent.remove(0, ancestors)
                return

            # fall back to the left child
            additional_ancestors = [(self, index)]
            descendent = self.children[index]
            while descendent.children:
                additional_ancestors.append(
                        (descendent, len(descendent.children) - 1))
                descendent = descendent.children[-1]
            ancestors.extend(additional_ancestors)
            self.contents[index] = descendent.contents[-1]
            descendent.remove(len(descendent.children) - 1, ancestors)
        else:
            self.contents.pop(index)
            if len(self.contents) < minimum and ancestors:
                self.grow(ancestors)

class BTree(object):
    BRANCH = LEAF = _BNode

    def __init__(self, order):
        self.order = order
        self._root = self._bottom = self.LEAF(self)

    def _path_to(self, item):
        current = self._root
        ancestry = []

        while getattr(current, "children", None):
            index = bisect.bisect_left(current.contents, item)
            ancestry.append((current, index))
            if index < len(current.contents) \
                    and current.contents[index] == item:
                return ancestry
            current = current.children[index]

        index = bisect.bisect_left(current.contents, item)
        ancestry.append((current, index))
        present = index < len(current.contents)
        present = present and current.contents[index] == item

        return ancestry

    def _present(self, item, ancestors):
        last, index = ancestors[-1]
        return index < len(last.contents) and last.contents[index] == item

    def insert(self, item):
        current = self._root
        ancestors = self._path_to(item)
        node, index = ancestors[-1]
        while getattr(node, "children", None):
            node = node.children[index]
            index = bisect.bisect_left(node.contents, item)
            ancestors.append((node, index))
        node, index = ancestors.pop()
        node.insert(index, item, ancestors)

    def remove(self, item):
        current = self._root
        ancestors = self._path_to(item)

        if self._present(item, ancestors):
            node, index = ancestors.pop()
            node.remove(index, ancestors)
        else:
            raise ValueError("%r not in %s" % (item, self.__class__.__name__))

    def __contains__(self, item):
        return self._present(item, self._path_to(item))

    def __iter__(self):
        def _recurse(node):
            if node.children:
                for child, item in zip(node.children, node.contents):
                    for child_item in _recurse(child):
                        yield child_item
                    yield item
                for child_item in _recurse(node.children[-1]):
                    yield child_item
            else:
                for item in node.contents:
                    yield item

        for item in _recurse(self._root):
            yield item

    def __repr__(self):
        def recurse(node, accum, depth):
            accum.append(("  " * depth) + repr(node))
            for node in getattr(node, "children", []):
                recurse(node, accum, depth + 1)

        accum = []
        recurse(self._root, accum, 0)
        return "\n".join(accum)

    @classmethod
    def bulkload(cls, items, order):
        tree = object.__new__(cls)
        tree.order = order

        leaves = tree._build_bulkloaded_leaves(items)
        tree._build_bulkloaded_branches(leaves)

        return tree

    def _build_bulkloaded_leaves(self, items):
        minimum = self.order // 2
        leaves, seps = [[]], []

        for item in items:
            if len(leaves[-1]) < self.order:
                leaves[-1].append(item)
            else:
                seps.append(item)
                leaves.append([])

        if len(leaves[-1]) < minimum and seps:
            last_two = leaves[-2] + [seps.pop()] + leaves[-1]
            leaves[-2] = last_two[:minimum]
            leaves[-1] = last_two[minimum + 1:]
            seps.append(last_two[minimum])

        return [self.LEAF(self, contents=node) for node in leaves], seps

    def _build_bulkloaded_branches(self, (leaves, seps)):
        minimum = self.order // 2
        levels = [leaves]

        while len(seps) > self.order + 1:
            items, nodes, seps = seps, [[]], []

            for item in items:
                if len(nodes[-1]) < self.order:
                    nodes[-1].append(item)
                else:
                    seps.append(item)
                    nodes.append([])

            if len(nodes[-1]) < minimum and seps:
                last_two = nodes[-2] + [seps.pop()] + nodes[-1]
                nodes[-2] = last_two[:minimum]
                nodes[-1] = last_two[minimum + 1:]
                seps.append(last_two[minimum])

            offset = 0
            for i, node in enumerate(nodes):
                children = levels[-1][offset:offset + len(node) + 1]
                nodes[i] = self.BRANCH(self, contents=node, children=children)
                offset += len(node) + 1

            levels.append(nodes)

        self._root = self.BRANCH(self, contents=seps, children=levels[-1])

import random
import unittest


class BTreeTests(unittest.TestCase):
    def test_additions(self):
        bt = BTree(20)
        l = range(2000)
        for i, item in enumerate(l):
            bt.insert(item)
            self.assertEqual(list(bt), l[:i + 1])

    def test_bulkloads(self):
        bt = BTree.bulkload(range(2000), 20)
        self.assertEqual(list(bt), range(2000))

    def test_removals(self):
        bt = BTree(20)
        l = range(2000)
        map(bt.insert, l)
        rand = l[:]
        random.shuffle(rand)
        while l:
            self.assertEqual(list(bt), l)
            rem = rand.pop()
            l.remove(rem)
            bt.remove(rem)
        self.assertEqual(list(bt), l)

    def test_insert_regression(self):
        bt = BTree.bulkload(range(2000), 50)

        for i in xrange(100000):
            bt.insert(random.randrange(2000))

if __name__ == '__main__':
    #unittest.main()
    b = BTree(2)
    for i in xrange(0,20):
        b.insert(i)
    print b
```

KD tree:
k-d tree (short for k-dimensional tree) is a space-partitioning data structure for organizing points in a k-dimensional space. k-d trees are a useful data structure for several applications, such as searches involving a multidimensional search key (e.g. range searches and nearest neighbor searches). k-d trees are a special case of binary space partitioning trees.

Time complexity in big O notation
Algorithm	Average		Worst Case
Space		O(n)		O(n)
Search		O(log n)	O(n)
Insert		O(log n)	O(n)
Delete		O(log n)	O(n)

examples of use: 
I've used them as an efficient way to find the nearest neighbors of a given point 
, you could use a kd-tree to store a collection of points in the Cartesian plane, in
three-dimensional space, etc. You could also use a kd-tree to store biometric data, for example, by
representing the data as an ordered tuple, perhaps (height, weight, blood pressure, cholesterol).
However, a kd-tree cannot be used to store collections of other data types, such as strings. Also
note that while it's possible to build a kd-tree to hold data of any dimension, all of the data stored in
a kd-tree must have the same dimension

https://www.youtube.com/watch?v=TLxWtXEbtFE

example:

![KD Tree ex](/../master/images/kdtree1.png?raw=true "KD tree ex")
![KD Tree ex](/../master/images/kdtree2.png?raw=true "KD tree ex")
![KD Tree ex](/../master/images/kdtree3.png?raw=true "KD tree ex")
![KD Tree ex](/../master/images/kdtree4.png?raw=true "KD tree ex")
![KD Tree ex](/../master/images/kdtree5.png?raw=true "KD tree ex")
![KD Tree ex](/../master/images/kdtree6.png?raw=true "KD tree ex")
![KD Tree ex](/../master/images/kdtree7.png?raw=true "KD tree ex")


```python 
from collections import namedtuple
from operator import itemgetter
from pprint import pformat

class Node(namedtuple('Node', 'location left_child right_child')):
    def __repr__(self):
        return pformat(tuple(self))

def kdtree(point_list, depth=0):
    try:
        k = len(point_list[0]) # assumes all points have the same dimension
    except IndexError as e: # if not point_list:
        return None
    # Select axis based on depth so that axis cycles through all valid values
    axis = depth % k
 
    # Sort point list and choose median as pivot element
    point_list.sort(key=itemgetter(axis))
    median = len(point_list) // 2 # choose median
 
    # Create node and construct subtrees
    return Node(
        location=point_list[median],
        left_child=kdtree(point_list[:median], depth + 1),
        right_child=kdtree(point_list[median + 1:], depth + 1)
    )

def main():
    """Example usage"""
    point_list = [(2,3), (5,4), (9,6), (4,7), (8,1), (7,2)]
    tree = kdtree(point_list)
    print(tree)

if __name__ == '__main__':
    main()
```
```
Output would be:
((7, 2),
 ((5, 4), ((2, 3), None, None), ((4, 7), None, None)),
 ((9, 6), ((8, 1), None, None), None))
```

## 9. Graphs
* 3 basic ways to represent a graph in memory (objects and pointers, matrix, and adjacency list); familiarize yourself with each representation and its pros & cons

look at three criteria. One is how much memory, or space, we need in each representation. We'll use asymptotic notation for that. Yes, we can use asymptotic notation for purposes other than expressing running times! It's really a way to characterize functions, and a function can describe a running time, an amount of space required, or some other resource. The other two criteria we'll use relate to time. One is how long it takes to determine whether a given edge is in the graph. The other is how long it takes to find the neighbors of a given vertex.

![Social Networking](/../master/images/socialnetworkgraph.png?raw=true "Social Networking Graph")

### Edge List
One simple way to represent a graph is just a list, or array, of |E| ∣E∣vertical bar, E, vertical bar edges, which we call an edge list. To represent an edge, we just have an array of two vertex numbers, or an array of objects containing the vertex numbers of the vertices that the edges are incident on. If edges have weights, add either a third element to the array or more information to the object, giving the edge's weight. Since each edge contains just two or three numbers, the total space for an edge list is \Theta(E) Θ(E). For example, here's how we represent an edge list in JavaScript for the social network graph:

```javascript 
[ [0,1], [0,6], [0,8], [1,4], [1,6], [1,9], [2,4], [2,6], [3,4], [3,5],
[3,8], [4,5], [4,9], [7,8], [7,9] ]
```
Edge lists are simple, but if we want to find whether the graph contains a particular edge, we have to search through the edge list. If the edges appear in the edge list in no particular order, that's a linear search through |E| ∣E∣vertical bar, E, vertical bar edges. Question to think about: How can you organize an edge list to make searching for a particular edge take O(\lg E) O(lgE) time? The answer is a little tricky.

### Adjacency matrices
For a graph with |V| ∣V∣vertical bar, V, vertical bar vertices, an adjacency matrix is a |V| \times |V| ∣V∣×∣V∣vertical bar, V, vertical bar, times, vertical bar, V, vertical bar matrix of 0s and 1s, where the entry in row i ii and column j jj is 1 if and only if the edge (i,j) (i,j)left parenthesis, i, comma, j, right parenthesis is in the graph. If you want to indicate an edge weight, put it in the row i ii, column j jj entry, and reserve a special value (perhaps null) to indicate an absent edge. Here's the adjacency matrix for the social network graph:

![Adjaceny Matrices](/../master/images/adjacencyMatrices.png?raw=true "Adjaceny Matrices")

```javascript 
[ [0, 1, 0, 0, 0, 0, 1, 0, 1, 0],
  [1, 0, 0, 0, 1, 0, 1, 0, 0, 1],
  [0, 0, 0, 0, 1, 0, 1, 0, 0, 0],
  [0, 0, 0, 0, 1, 1, 0, 0, 1, 0],
  [0, 1, 1, 1, 0, 1, 0, 0, 0, 1],
  [0, 0, 0, 1, 1, 0, 0, 0, 0, 0],
  [1, 1, 1, 0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0, 0, 1, 1],
  [1, 0, 0, 1, 0, 0, 0, 1, 0, 0],
  [0, 1, 0, 0, 1, 0, 0, 1, 0, 0] ]
```
With an adjacency matrix, we can find out whether an edge is present in constant time, by just looking up the corresponding entry in the matrix. For example, if the adjacency matrix is named graph, then we can query whether edge (i,j) (i,j)left parenthesis, i, comma, j, right parenthesis is in the graph by looking at graph[i][j]. So what's the disadvantage of an adjacency matrix? Two things, actually. First, it takes \Theta(V^2) Θ(V
​2
​​ ) space, even if the graph is sparse: relatively few edges. In other words, for a sparse graph, the adjacency matrix is mostly 0s, and we use lots of space to represent only a few edges. Second, if you want to find out which vertices are adjacent to a given vertex i ii, you have to look at all |V| ∣V∣vertical bar, V, vertical bar entries in row i ii, even if only a small number of vertices are adjacent to vertex i ii.
For an undirected graph, the adjacency matrix is symmetric: the row i ii, column j jj entry is 1 if and only if the row j jj, column i ii entry is 1. For a directed graph, the adjacency matrix need not be symmetric

### Adjacency List
Representing a graph with adjacency lists combines adjacency matrices with edge lists. For each vertex i ii, store an array of the vertices adjacent to it. We typically have an array of |V| ∣V∣vertical bar, V, vertical bar adjacency lists, one adjacency list per vertex. Here's an adjacency-list representation of the social network graph:

![Adjaceny List](/../master/images/adjacencyList.png?raw=true "Adjaceny List")

```javascript
[ [1, 6, 8],
  [0, 4, 6, 9],
  [4, 6],
  [4, 5, 8],
  [1, 2, 3, 5, 9],
  [3, 4],
  [0, 1, 2],
  [8, 9],
  [0, 3, 7],
  [1, 4, 7] ]
```

Vertex numbers in an adjacency list are not required to appear in any particular order, though it is often convenient to list them in increasing order, as in this example.
We can get to each vertex's adjacency list in constant time, because we just have to index into an array. To find out whether an edge (i,j) (i,j)left parenthesis, i, comma, j, right parenthesis is present in the graph, we go to i ii's adjacency list in constant time and then look for j jj in i ii's adjacency list. How long does that take in the worst case? The answer is \Theta(d) Θ(d), where d dd is the degree of vertex i ii, because that's how long i ii's adjacency list is. The degree of vertex i ii could be as high as |V|-1 ∣V∣−1vertical bar, V, vertical bar, minus, 1 (if i ii is adjacent to all the other |V|-1 ∣V∣−1vertical bar, V, vertical bar, minus, 1 vertices) or as low as 0 (if i ii is isolated, with no incident edges). In an undirected graph, vertex j jj is in vertex i ii's adjacency list if and only if i ii is in j jj's adjacency list. If the graph is weighted, then each item in each adjacency list is either a two-item array or an object, giving the vertex number and the edge weight.
You can use a for-loop to iterate through the vertices in an adjacency list. For example, suppose that you have an adjacency-list representation of a graph in the variable graph, so that graph[i] is an array containing the neighbors of vertex i ii. Then, to call a function doStuff on each vertex adjacent to vertex i ii, you could use the following JavaScript code:

```javascript
for (var j = 0; j < graph[i].length; j++) {
    doStuff(graph[i][j]);
}
```
If the double-subscript notation confuses you, you can think of it this way:

```javascript
var vertex = graph[i];
for (var j = 0; j < vertex.length; j++) {
    doStuff(vertex[j]);
}
```

How much space do adjacency lists take? We have |V| ∣V∣vertical bar, V, vertical bar lists, and although each list could have as many as |V|-1 ∣V∣−1vertical bar, V, vertical bar, minus, 1 vertices, in total the adjacency lists for an undirected graph contain 2|E| 2∣E∣2, vertical bar, E, vertical bar elements. Why 2|E| 2∣E∣2, vertical bar, E, vertical bar? Each edge (i,j) (i,j)left parenthesis, i, comma, j, right parenthesis appears exactly twice in the adjacency lists, once in i ii's list and once in j jj's list, and there are |E| ∣E∣vertical bar, E, vertical bar edges. For a directed graph, the adjacency lists contain a total of |E| ∣E∣vertical bar, E, vertical bar elements, one element per directed edge.

* You should know the basic graph traversal algorithms: breadth-first search and depth-first search
### breadth-first search
Breadth first search (BFS) is one of the easiest algorithms for searching a graph. It also serves as a prototype for several other important graph algorithms
Given a graph GG and a starting vertex ss, a breadth first search proceeds by exploring edges in the graph to find all the vertices in GG for which there is a path from ss. The remarkable thing about a breadth first search is that it finds all the vertices that are a distance kk from ss before it finds any vertices that are a distance k+1k+1. One good way to visualize what the breadth first search algorithm does is to imagine that it is building a tree, one level of the tree at a time. A breadth first search adds all children of the starting vertex before it begins to discover any of the grandchildren.

To keep track of its progress, BFS colors each of the vertices white, gray, or black. All the vertices are initialized to white when they are constructed. A white vertex is an undiscovered vertex. When a vertex is initially discovered it is colored gray, and when BFS has completely explored a vertex it is colored black. This means that once a vertex is colored black, it has no white vertices adjacent to it. A gray node, on the other hand, may have some white vertices adjacent to it, indicating that there are still additional vertices to explore.

```python
# python  breadth first search algorithm
# adjacency list with a Queue 

from pythonds.graphs import Graph, Vertex
from pythonds.basic import Queue

def bfs(g,start):
  start.setDistance(0)
  start.setPred(None)
  vertQueue = Queue()
  vertQueue.enqueue(start)
  while (vertQueue.size() > 0):
    currentVert = vertQueue.dequeue()
    for nbr in currentVert.getConnections():
      if (nbr.getColor() == 'white'):
        nbr.setColor('gray')
        nbr.setDistance(currentVert.getDistance() + 1)
        nbr.setPred(currentVert)
        vertQueue.enqueue(nbr)
    currentVert.setColor('black')
```
BFS begins at the starting vertex s and colors start gray to show that it is currently being explored. Two other values, the distance and the predecessor, are initialized to 0 and None respectively for the starting vertex. Finally, start is placed on a Queue. The next step is to begin to systematically explore vertices at the front of the queue. We explore each new node at the front of the queue by iterating over its adjacency list. As each node on the adjacency list is examined its color is checked. If it is white, the vertex is unexplored, and four things happen:

1. The new, unexplored vertex nbr, is colored gray.
2. The predecessor of nbr is set to the current node currentVert
3. The distance to nbr is set to the distance to currentVert + 1
4. nbr is added to the end of a queue. Adding nbr to the end of the queue effectively schedules this node for further exploration, but not until all the other vertices on the adjacency list of currentVert have been explored.


the Graph
![The Graph](/../master/images/theGraph.png?raw=true "The Graph")

 Starting from fool we take all nodes that are adjacent to fool and add them to the tree. The adjacent nodes include pool, foil, foul, and cool. Each of these nodes are added to the queue of new nodes to expand.
 
 ![BFS](/../master/images/bfs1.png?raw=true "BFS")
 
 In the next step bfs removes the next node (pool) from the front of the queue and repeats the process for all of its adjacent nodes. However, when bfs examines the node cool, it finds that the color of cool has already been changed to gray. This indicates that there is a shorter path to cool and that cool is already on the queue for further expansion. The only new node added to the queue while examining pool is poll. 
 
 ![BFS](/../master/images/bfs2.png?raw=true "BFS")
 
The next vertex on the queue is foil. The only new node that foil can add to the tree is fail. As bfs continues to process the queue, neither of the next two nodes add anything new to the queue or the tree.

![BFS](/../master/images/bfs3.png?raw=true "BFS")

![BFS](/../master/images/bfs4.png?raw=true "BFS")

### depth-first search

depth first search where the goal is to create the deepest depth first tree, without any branches. The more general depth first search is actually easier. Its goal is to search as deeply as possible, connecting as many nodes in the graph as possible and branching where necessary.

It is even possible that a depth first search will create more than one tree. When the depth first search algorithm creates a group of trees we call this a depth first forest. As with the breadth first search our depth first search makes use of predecessor links to construct the tree. In addition, the depth first search will make use of two additional instance variables in the Vertex class. The new instance variables are the discovery and finish times. The discovery time tracks the number of steps in the algorithm before a vertex is first encountered. The finish time is the number of steps in the algorithm before a vertex is colored black. As we will see after looking at the algorithm, the discovery and finish times of the nodes provide some interesting properties we can use in later algorithms.

```python
from pythonds.graphs import Graph
class DFSGraph(Graph):
    def __init__(self):
        super().__init__()
        self.time = 0

    def dfs(self):
        for aVertex in self:
            aVertex.setColor('white')
            aVertex.setPred(-1)
        for aVertex in self:
            if aVertex.getColor() == 'white':
                self.dfsvisit(aVertex)

    def dfsvisit(self,startVertex):
        startVertex.setColor('gray')
        self.time += 1
        startVertex.setDiscovery(self.time)
        for nextVertex in startVertex.getConnections():
            if nextVertex.getColor() == 'white':
                nextVertex.setPred(startVertex)
                self.dfsvisit(nextVertex)
        startVertex.setColor('black')
        self.time += 1
        startVertex.setFinish(self.time)
```

Since the two functions dfs and its helper dfsvisit use a variable to keep track of the time across calls to dfsvisit we chose to implement the code as methods of a class that inherits from the Graph class. This implementation extends the graph class by adding a time instance variable and the two methods dfs and dfsvisit. Looking at line 11 you will notice that the dfs method iterates over all of the vertices in the graph calling dfsvisit on the nodes that are white. The reason we iterate over all the nodes, rather than simply searching from a chosen starting node, is to make sure that all nodes in the graph are considered and that no vertices are left out of the depth first forest. It may look unusual to see the statement for aVertex in self, but remember that in this case self is an instance of the DFSGraph class, and iterating over all the vertices in an instance of a graph is a natural thing to do.

The dfsvisit method starts with a single vertex called startVertex and explores all of the neighboring white vertices as deeply as possible. If you look carefully at the code for dfsvisit and compare it to breadth first search, what you should notice is that the dfsvisit algorithm is almost identical to bfs except that on the last line of the inner for loop, dfsvisit calls itself recursively to continue the search at a deeper level, whereas bfs adds the node to a queue for later exploration. It is interesting to note that where bfs uses a queue, dfsvisit uses a stack. You don’t see a stack in the code, but it is implicit in the recursive call to dfsvisit.

The following sequence of figures illustrates the depth first search algorithm in action for a small graph. In these figures, the dotted lines indicate edges that are checked, but the node at the other end of the edge has already been added to the depth first tree. In the code this test is done by checking that the color of the other node is non-white.

The search begins at vertex A of the graph (Figure 14). Since all of the vertices are white at the beginning of the search the algorithm visits vertex A. The first step in visiting a vertex is to set the color to gray, which indicates that the vertex is being explored and the discovery time is set to 1. Since vertex A has two adjacent vertices (B, D) each of those need to be visited as well. We’ll make the arbitrary decision that we will visit the adjacent vertices in alphabetical order.

Vertex B is visited next (Figure 15), so its color is set to gray and its discovery time is set to 2. Vertex B is also adjacent to two other nodes (C, D) so we will follow the alphabetical order and visit node C next.

Visiting vertex C (Figure 16) brings us to the end of one branch of the tree. After coloring the node gray and setting its discovery time to 3, the algorithm also determines that there are no adjacent vertices to C. This means that we are done exploring node C and so we can color the vertex black, and set the finish time to 4. You can see the state of our search at this point in Figure 17.

Since vertex C was the end of one branch we now return to vertex B and continue exploring the nodes adjacent to B. The only additional vertex to explore from B is D, so we can now visit D (Figure 18) and continue our search from vertex D. Vertex D quickly leads us to vertex E (Figure 19). Vertex E has two adjacent vertices, B and F. Normally we would explore these adjacent vertices alphabetically, but since B is already colored gray the algorithm recognizes that it should not visit B since doing so would put the algorithm in a loop! So exploration continues with the next vertex in the list, namely F (Figure 20).

Vertex F has only one adjacent vertex, C, but since C is colored black there is nothing else to explore, and the algorithm has reached the end of another branch. From here on, you will see in Figure 21 through Figure 25  that the algorithm works its way back to the first node, setting finish times and coloring vertices black.

![DFS](/../master/images/DFS1.png?raw=true "DFS")
![DFS](/../master/images/DFS2.png?raw=true "DFS")
![DFS](/../master/images/DFS3.png?raw=true "DFS")

The starting and finishing times for each node display a property called the parenthesis property. This property means that all the children of a particular node in the depth first tree have a later discovery time and an earlier finish time than their parent

![DFS](/../master/images/DFS4.png?raw=true "DFS")

* Know their computational complexity, their tradeoffs, and how to implement them in real code
BFS = 
Class				Search algorithm
Data structure			Graph
Worst-case performance	 	O(|V|+|E|)=O(b^{d})
Worst-case space complexity  	O(|V|)=O(b^{d})

BFS is usually implemented with  Queue

is going to use more memory depending on the branching factor... however, BFS is a complete algorithm... meaning if you are using it to search for something in the lowest depth possible, BFS will give you the optimal solution. BFS space complexity is O(b^d)... the branching factor raised to the depth (can be A LOT of memory).
see code above

DFS = 
Class 				Search algorithm
Data structure			Graph
Worst-case performance		O(|V| + |E|) for explicit graphs traversed without repetition, O(b^d) for implicit graphs with branching factor b searched to depth d
Worst-case space complexity	 O(|V|) if entire graph is traversed without repetition, O(longest path length searched) for implicit graphs without elimination of duplicate nodes

DFS uses a Stack

DFS on the other hand, is much better about space however it may find a suboptimal solution. Meaning, if you are just searching for a path from one vertex to another, you may find the suboptimal solution (and stop there) before you find the real shortest path. DFS space complexity is  O(|V|)... meaning that the most memory it can take up is the longest possible path.
see code above

* study up on fancier algorithms, such as Dijkstra and A\*.

#### Dijkstra
is an algorithm for finding the shortest paths between nodes in a graph, which may represent, for example, road networks. 

Dijkstra's algorithm to find the shortest path between a and b. It picks the unvisited vertex with the lowest distance, calculates the distance through it to each unvisited neighbor, and updates the neighbor's distance if smaller. Mark visited (set to red) when done with neighbors.
Class			Search algorithm
Data structure		Graph
Worst-case performance	O(|E|+|V|\log |V|)

![Dijkstra](/../master/images/dijkstra1.png?raw=true "Dijkstra ex")
![Dijkstra](/../master/images/dijkstra2.png?raw=true "Dijkstra ex")
![Dijkstra](/../master/images/dijkstra3.png?raw=true "Dijkstra ex")
![Dijkstra](/../master/images/dijkstra4.png?raw=true "Dijkstra ex")
![Dijkstra](/../master/images/dijkstra5.png?raw=true "Dijkstra ex")
![Dijkstra](/../master/images/dijkstra6.png?raw=true "Dijkstra ex")
![Dijkstra](/../master/images/dijkstra7.png?raw=true "Dijkstra ex")
![Dijkstra](/../master/images/dijkstra8.png?raw=true "Dijkstra ex")
![Dijkstra](/../master/images/dijkstra9.png?raw=true "Dijkstra ex")
![Dijkstra](/../master/images/dijkstra10.png?raw=true "Dijkstra ex")
![Dijkstra](/../master/images/dijkstra11.png?raw=true "Dijkstra ex")

```python
class Graph:
  def __init__(self):
    self.nodes = set()
    self.edges = defaultdict(list)
    self.distances = {}

  def add_node(self, value):
    self.nodes.add(value)

  def add_edge(self, from_node, to_node, distance):
    self.edges[from_node].append(to_node)
    self.edges[to_node].append(from_node)
    self.distances[(from_node, to_node)] = distance


def dijsktra(graph, initial):
  visited = {initial: 0}
  path = {}

  nodes = set(graph.nodes)

  while nodes: 
    min_node = None
    for node in nodes:
      if node in visited:
        if min_node is None:
          min_node = node
        elif visited[node] < visited[min_node]:
          min_node = node

    if min_node is None:
      break

    nodes.remove(min_node)
    current_weight = visited[min_node]

    for edge in graph.edges[min_node]:
      weight = current_weight + graph.distance[(min_node, edge)]
      if edge not in visited or weight < visited[edge]:
        visited[edge] = weight
        path[edge] = min_node

  return visited, path
```
#### A\*
 video = https://www.youtube.com/watch?v=eTx6HQ9Veas

 is a computer algorithm that is widely used in pathfinding and graph traversal, the process of plotting an efficiently directed path between multiple points, called nodes. It enjoys widespread use due to its performance and accuracy. However, in practical travel-routing systems, it is generally outperformed by algorithms which can pre-process the graph to attain better performance,[1] although other work has found A* to be superior to other approaches
 
 A* is an informed search algorithm, or a best-first search, meaning that it solves problems by searching among all possible paths to the solution (goal) for the one that incurs the smallest cost (least distance travelled, shortest time, etc.), and among these paths it first considers the ones that appear to lead most quickly to the solution. It is formulated in terms of weighted graphs: starting from a specific node of a graph, it constructs a tree of paths starting from that node, expanding paths one step at a time, until one of its paths ends at the predetermined goal node.

At each iteration of its main loop, A* needs to determine which of its partial paths to expand into one or more longer paths. It does so based on an estimate of the cost (total weight) still to go to the goal node. Specifically, A* selects the path that minimizes

{\displaystyle f(n)=g(n)+h(n)} f(n)=g(n)+h(n)
where n is the last node on the path, g(n) is the cost of the path from the start node to n, and h(n) is a heuristic that estimates the cost of the cheapest path from n to the goal. The heuristic is problem-specific. For the algorithm to find the actual shortest path, the heuristic function must be admissible, meaning that it never overestimates the actual cost to get to the nearest goal node.

Typical implementations of A* use a priority queue to perform the repeated selection of minimum (estimated) cost nodes to expand. This priority queue is known as the open set or fringe. At each step of the algorithm, the node with the lowest f(x) value is removed from the queue, the f and g values of its neighbors are updated accordingly, and these neighbors are added to the queue. The algorithm continues until a goal node has a lower f value than any node in the queue (or until the queue is empty).[a] The f value of the goal is then the length of the shortest path, since h at the goal is zero in an admissible heuristic.

The algorithm described so far gives us only the length of the shortest path. To find the actual sequence of steps, the algorithm can be easily revised so that each node on the path keeps track of its predecessor. After this algorithm is run, the ending node will point to its predecessor, and so on, until some node's predecessor is the start node.

As an example, when searching for the shortest route on a map, h(x) might represent the straight-line distance to the goal, since that is physically the smallest possible distance between any two points.

If the heuristic h satisfies the additional condition h(x) ≤ d(x, y) + h(y) for every edge (x, y) of the graph (where d denotes the length of that edge), then h is called monotone, or consistent. In such a case, A* can be implemented more efficiently—roughly speaking, no node needs to be processed more than once (see closed set below)—and A* is equivalent to running Dijkstra's algorithm with the reduced cost d'(x, y) = d(x, y) + h(y) − h(x).

```
# Pseudocode
function A*(start, goal)
    // The set of nodes already evaluated.
    closedSet := {}
    // The set of currently discovered nodes that are already evaluated.
    // Initially, only the start node is known.
    openSet := {start}
    // For each node, which node it can most efficiently be reached from.
    // If a node can be reached from many nodes, cameFrom will eventually contain the
    // most efficient previous step.
    cameFrom := the empty map

    // For each node, the cost of getting from the start node to that node.
    gScore := map with default value of Infinity
    // The cost of going from start to start is zero.
    gScore[start] := 0 
    // For each node, the total cost of getting from the start node to the goal
    // by passing by that node. That value is partly known, partly heuristic.
    fScore := map with default value of Infinity
    // For the first node, that value is completely heuristic.
    fScore[start] := heuristic_cost_estimate(start, goal)

    while openSet is not empty
        current := the node in openSet having the lowest fScore[] value
        if current = goal
            return reconstruct_path(cameFrom, current)

        openSet.Remove(current)
        closedSet.Add(current)
        for each neighbor of current
            if neighbor in closedSet
                continue		// Ignore the neighbor which is already evaluated.
            // The distance from start to a neighbor
            tentative_gScore := gScore[current] + dist_between(current, neighbor)
            if neighbor not in openSet	// Discover a new node
                openSet.Add(neighbor)
            else if tentative_gScore >= gScore[neighbor]
                continue		// This is not a better path.

            // This path is the best until now. Record it!
            cameFrom[neighbor] := current
            gScore[neighbor] := tentative_gScore
            fScore[neighbor] := gScore[neighbor] + heuristic_cost_estimate(neighbor, goal)

    return failure

function reconstruct_path(cameFrom, current)
    total_path := [current]
    while current in cameFrom.Keys:
        current := cameFrom[current]
        total_path.append(current)
    return total_path
```


## 10. Other data structures
* know about the most famous classes of NP-complete problems, such as traveling salesman and the knapsack problem, and be able to recognize them when an interviewer asks you them in disguise.
* Find out what NP-complete means

## 11 Mathmatics
* Spend some time before the interview refreshing your memory on (or teaching yourself) the essentials of combinatorics and probability. You should be familiar with n-choose-k problems and their ilk – the more the better.

## 12 Operating Syatems
* Know about processes, threads and concurrency issues
* Know about locks and mutexes and semaphores and monitors and how they work
* Know about deadlock and livelock and how to avoid them
* Know what resources a processes needs, and a thread needs, and how context switching works, and how it's initiated by the operating system and underlying hardware
* Know a little about scheduling. 
* The world is rapidly moving towards multi-core, so know the fundamentals of "modern" concurrency constructs.
* resource = http://research.google.com/pubs/DistributedSystemsandParallelComputing.html

## 13 Important reaserch publications:
*  http://research.google.com/pubs/papers.html
* paper on Google’s Hybrid Approach to Research:
http://static.googleusercontent.com/media/research.google.com/en/us/pubs/archive/38149.pdf

## 14 Must reads
* Programming Interviews Exposed: Secrets to Landing Your Next Job by John Mongan and Noah Suojanen
* "Programming Pearls" by Jon Bentley
* "Cormen/Leiserson/Rivest/Stein: Introduction to Algorithms" 

## 15 online resourse
* www.topcoder.com - launch the area widget and then go to the practice rooms where you can play the problem in the  first/second division as a warm up
* projecteuler.net - programming problems you wont normally come across 

## 16 tips for interviewing at google
* https://www.youtube.com/watch?v=w887NIa_V9w

## 17 google products
*  https://www.google.com/intl/en/about/products/

## 18 5 essental Phone screen questions by Steve Yegge(google engineer)
* https://sites.google.com/site/steveyegge2/five-essential-phone-screen-questions

## 19 Types of algorithum question google asks: topCider Tutorials 
* https://www.topcoder.com/community/data-science/data-science-tutorials/

## 20 The Official Google Blog: "Baby steps to a new job" by Gretta Cook(google engineer)

## 21 “How to Get Hired” by Dan Kegel (Google Engineer)
* https://www.youtube.com/watch?v=qc1owf2-220

## 22 "google recruiters share Technical Interview Tips
* https://www.youtube.com/watch?v=qc1owf2-220

## 23 Candidate Coaching Session: Tech Interviewing
*  https://www.youtube.com/watch?v=oWbUtlUhwa8

## 24 Interviewing at Google
* https://www.youtube.com/watch?v=w887NIa_V9w

## 25 extra credit
* 1 - Interview Cake sends you a weekly question  https://www.interviewcake.com
* 2 - Daily practice questions https://leetcode.com/ 
* 3 - Pramp helps with mock interviews   https://www.pramp.com 
* 4 - clear code: A handbook of agile software craftsmanship http://ricardogeek.com/docs/clean_code.pdf

## sample question 

### system design
examples: 
question - 
You have 10^9 user, 10^3 websites that users are subscribed to and 2000 servers. Some users will unsubscribe from certain websites. How would you architect this system to be scalable and performant?

answers I liked  (not mine) - 
What you probably got to do is create a data structure which will capture the user input (unsubs / subs), and subsequently apply to the existing user data in certain scalable fashion which varies on what basis the data is being used.

We need to have a lookup table where they key is userId and value is a list of the websites that user is subscribed to. Let's assume we assign each user a 32-bit(4 byte) int user iD. Let's also assign 4 byte int iDs to the websites. If in the worst case a user is subscribed to all websites, a single key-value pair will take up 16Kb of space we have a total of 16Kb * 10^9 users = 1.6 * 10^12 bytes of data (1.6TB). Assume that a single server can store 8GB of data then 1.6 * 10^12 / 8 * 10^9 = 2 * 10^3 servers ( which is what we have). This allows us to store data on 500K users per server. Each server can store a range of User IDs (ex. server 1 stores IDs from 1...499 999). When a user unsubscribes based on his userId we can navigate to the correct server containing the corresponding look-up table. In the look up table delete the ID of the website he wishes to unsubscribe from.

question - how do you design a systemn fir very large graphs(does not fit in a single machine)
answers I liked - 
Adj list, but in following way:
Distributed hash map.
A key is either a node or pair of nodes.
If it is a node, a value is a pair that consists of key node and the first node in adjacency list.
If it is a pair. The key consists of node for who the adjacency list and the current node in adj list. A value consists of node for who the adjacency list and the next node in adj list.

question - how would you store the relations in a social network like Facebook and implemet a featire where one user recieves notifications whern their friends like the same thing as they do

answers: 
You would store the relations in a Graph Database (like Facebook actually does) and attach a trigger to the liking of a page to send a notification to all their friends who also liked that page (really easy to do in a graph database)


question:  Design Youtubeview-counting feature
answer: 
Always simplify things and start building up.

Assume that we have a small set of videos that we need to track the counts. Also assume that we are not handling any spam. A basic approach is using relational databases (video_id, count) that is being incremented whenever a user clicks on. 

What is the problem with this approach? The database operation is very expensive for each click. A possible solution we can have Memcache or Redis layer above the DB that counts the number of clicks for each video. And having a background job that reads the cached counts and reflects that in the DB. Pros: much faster, Cons: more complex, can lose data in case of server failure and cached values have not populated in the DB yet and the values in the DB are not the most recent values.

Another side of improvement is bandwidth wise. Assuem that it is not necessary that the number of hits to be really accurate. We can convert the connection request for increment to be based on UDP rather than TCP.

Again discuss with the interview the pros and cons and let him lead which direction should the discussion goes.


question: how would you optimize an elevator system for a building with 50 flors and 4 elevators? Optimize in terms of lowest wait times fo rthe users. Nothing related to power consumption.

answer: One idea is to minimize the time an elevator is spent moving while empty. Assuming an even distribution of people throughout the building and where they want to go, this might be satisfied by spreading out all elevators evenly among floors. ie. make it so elevators always come to rest at floors 1, 17, 33, 50.

We can also think of "a person pressing an up/down button" as a request. Every time an elevator has dropped off its last person and there are no outstanding requests, move it to the closest floor interval mentioned above that isn't already occupied by another elevator. If there are outstanding requests, address them in a way that maximizes the number of requests handled in a given direction that aren't already "on the way" for another elevator. There is probably some formal definition for this behaviour, but this is the general idea.

We can model the requests as 2 queues, where one is for requests going up, another for going down, and elevators having pointers that move along the queues, removing requests, and adding requests to themselves for processing.

For real world scenarios, we can apply heuristics such as people tend to go from the first floor up to other floors in the morning, and opposite in the evening. This would mean the morning would have elevator resting positions shifted towards the bottom half of the building Vs the opposite in the evening. During the day when movement is more evenly distributed, the elevators can also be evenly distributed.


comments- 
queue will be overkill and we don't want strict FIFO. Imagine an elevator is requested from 1 floor to 20 and in between 10 floor requests it. Each elevator maintains an array (as a bitmap)

generally the right idea. and I agree with , no queue. More importantly getting to know the current system is key before beginning to optimize it. For example, what does wait time actually mean? does it simply mean, calling for an elevator and getting into it? Coz, remember this is a really tall building and someone wanting to go to the higher floors from the ground floor might be experiencing a lot to transit time while the elevator picks and drops people on the way up. Your solution although generally good is not clearly addressing this.


question - what happens dirring and after a query is being typed(autocomplete) in a search box weather the user is trying to go to a website of asking questions etc. and how do servers complete the request and what is the best (parallel) structure for the request to go through. 

answesrs - 
Lets take example of Google page - auto suggestion. This is what I believe should be happening. 
1. When user enters into search box, the view module (ex jsp) makes a call the google webapp servlet with user inputs as parameter string using ofcourse AJAX. 
2. Based on the inputs / paramter string the server looks up the possible suggestion by looking up the data structure that stores all the previous searched strings. Data structure here would best be Trie. search is a DFS 
3. after completing all strings search, the servlet sends back the collection to view module. 
4. view on reciept of response, sets the respective DOM object to display the suggestion. 

Autocomplete can be achieved many different ways. Most common is Ajax on the client side, and an NLP search engine application like Apache Lucene or Solr that does a reverse lookup and returns the matches based on the typed characters. These search engines can be very finely tuned. The rest of the question is not clear.


question - The setup is that we are given a series of text files which contain information regarding a code repository's commits. Each file represents a single commit and they are formatted as follows: 
" 
Commit #: XXX 
Author: XXX 
Reviewer(s): XXX, XXX, ... 
File: XXX 
File: XXX 
... 
Date: XX:XX:XX XX/XX/XXXX 
" 
The commit number is unique and is generated in synchronous order. There is exactly 1 unique author. There are a variable number of reviewers, delimited by commas; if there are no reviewers, that line is absent from the file. There are a variable number of edited files in the commit, each receiving its own line. The time/date is when the commit was submitted. 

First design a graphical model for all of the commit data. Then describe how this model is updated when a new commit is generated. Finally, write the code segment called when a new commit is generated which edits a system that has implemented your model of the data - its input is a file name and whatever necessary data structures that are maintained by your system.

answer - 
1) Keep a list of managed files (say, L) and their status. 
2) Each file has a connection to all related commits. (commits are ranked based on time) 
3) Each commit has a connection to all related file (including not-changed files). 

For the latest version, scan the files and fetch the latest files. 
For one particular commit, scan the related files and fetch the files. 

Each time, when a new commit happens, 
1) If the commit new a file, insert the file to L. 
2) For all modified files, insert the commit into the beginning. 
3) Insert the commit to the ranked commit list.

question: 
design a system to return an unique ID for each request. For most of requests, the ID value should increase as time goes, the system should handle 1000 requests per second at least. 
timestamps alone is not valid since there might be multiple requests with same timestamps.

answer - 
Requirements: 

-> Unique Incrementing Id 
-> Scalable 
-> Available 
-> Consistent 

Solution: 

First of all lets determine a solution that allows you to get the best unique id that starts incrementing from 0. We can use the time stamp and convert to epoch to get a unique id. 
When we start this system we set a variable called START_EPOCH_TIME. 

During each request we calculate CURRENT_EPOCH_TIME - START_EPOCH_TIME. 

This should allow of us to get unique incrementing ids over the scale of the number of digits we use. 

However, this won't work when there are multiple requests within the same time stamp. So first thing is we modify the length of our id by the number of requests we want per second. i.e. ID000 for 1000. So what we do is maintain a hash table that we recycle on regular intervals that maps epoch time and count of the number of requests. We update the table on every hit of the request. 

REQ_COUNT [CURRENT_EPOCH_TIME] += 1 

Our unique ID is then ID + str(REQ_COUNT [CURRENT_EPOCH_TIME]) . 

If this is the only machine that handles requests then this shall work, however, this may not scale well when there are multiple machines handling requests. So lets say we have 10 machines handling requests. Then we use gossiping protocol to ensure that all the machines know about each other. This way we ensure that each machine knows exactly the number of machines that exist. Using this knowledge each machine assigns itself a range ordered by the machines IP/NAME. So for 10 machines. Machine 1 gets 0-100, 2 gets 100-200, 3 gets 200-300. 

Then the hash table will be initialized with: 
REQ_COUNT [CURRENT_EPOCH_TIME] = RANGE_START 

And incremented as such. 
REQ_COUNT [CURRENT_EPOCH_TIME] += 1. 

Finally we always check whether REQ_COUNT [CURRENT_EPOCH_TIME] > RANGE. If so then we throttle the request. Otherwise if we want to handle more we need to update the way the system works. This should be decided based on the requirements and capacity of the system in handling these requests.

question:
Given a large network of computers, each keeping log files of visited urls, find the top ten of the most visited urls. 
(i.e. have many large <string (url) -> int (visits)> maps, calculate implicitly <string (url) -> int (sum of visits among all distributed maps), and get the top ten in the combined map) 

The result list must be exact, and the maps are too large to transmit over the network (especially sending all of them to a central server or using MapReduce directly, is not allowed)

answer: 
Presuming a protocol exists that can ask three questions to each server: 

* the score of a single url 
* the top 10 
* the top n that satisfy score >= N 

We program a two pass solution like so: 

We denote the number of servers as S. 

[First pass] 
(1) Ask every server for its own top ten 

(2) merge the results. For all URLs in the merged set calculate correct values by asking 
all servers for their scores for each URL. Calculate a set of top ten from our sample. 

(3) pick score of the now tenth URL as the threshold that we try to beat 
in the second round. We denote the threshold as T. 

[Second pass] 
(4) Ask every server for all its top N that satisfy score >= T/S 

(5) Merge these bigger samples again as in step (2) 

(6) We now have the correct top ten with correct scores.

question:
How do you make sure an API does not leak memory?

answer: 
1> Design API such that it only Process the functionality and no memory allocation within it. 
2> Use the Smart pointer concept for every pointer usage within the API. 
3> Can make API's so that for every allocation there must be the deallocation called in. for every condition.

question: 
how you will design system for server and which will have many clients, all clients will keep of adding some words to system and server has to detect which words to accept, at the same time server will also keep on adding words.So which data structure you will use so that system will be efficient and fast.

answer: 
it can be considered as same client-server problem. 
where the same server is accessed by many clients at the same time. 

for this we can adopt the terminology to complete each client request on separate thread. and all the preprossing before adding that words (to check the validity,if the word is already present or not etc ) should be on this thread, and this thread respond to client asynchronously whether word is saved or not. 

ofcourse we should use proper locking method when accessing the common datastrucures from various threads. 

as far as datastructure is concerned for we can use hastables & tries. 
depending upon the requirement. 

hashtables has very fast has constant time access but require the very good hash function in advance. which can be difficult to estimate in advance. 

but tries has lgn access in all the cases.

question: 
	How do you design cache server for a simple web application. How do you make sure of the data consistancy. How do update your data/cache.

answer: 
You can design a cache server using a dynamic data structure like a double linked list. while you must be able to read the cache concurrently, only one thread or process must be able to write to it a time. this could be achieved by procuring specific read and write locks in the POSIX thread API.For cache eviction policy, LRU is a good choice and it can be implemented using counters.

