# P and NP Class Algorithms

![](https://lh5.googleusercontent.com/_QLECPt1M78b3U9SXf8E6dqmgxTKuRRdkSSv6D7kS_0g3h6nE2x9gnodvm7lZKRFFujprfP93WHGUDhvKB2PEwTPep-dq4qg1oTGNPX3jjPTnujilQA6iP_dWlWk2gTPRLu4ZoRr)![](https://lh3.googleusercontent.com/A6R9VBCtdV5o7X5qWEpB8vQLWgnHRdb10rIT4Zch2cbr0new8AZnZQJCy2mEhVMQaUHVWT5ptFXKepqn7H5QCNJlOAwwGi-VPChJEJ7S7n4PvPCWMZGsXuR6GbvaMCcPyrDehbgp)  
  
  
  
**Problem Types**

-   Optimization Problems 
-   Decision Problems
    

## Optimization Problem

Optimization problems are those for which the objective is to maximize or minimize some values. For example,

-   Finding the minimum number of colors needed to color a given graph.
-   Finding the shortest path between two vertices in a graph.
    

## Decision Problem

There are many problems for which the answer is a Yes or a No. These types of problems are known as decision problems. For example,

-   Whether a given graph can be colored by only 4-colors.
-   Finding Hamiltonian cycle in a graph is not a decision problem, whereas checking a graph is Hamiltonian or not is a decision problem.
    

Many problems are optimization Problems but we often recast as a Decision Problems

Example:

-   Optimization Problem: color the graph with minimum color in such a way that no two adjacent vertices have the same color. Convert it into decision problem
    
	-   Decision Problem: Is it possible to color a graph with 3 color??
    
there are many problems, such as traveling salesperson, optimal graph coloring, Hamiltonian cycles, finding the longest path in a graph, and satisfying a Boolean formula, for which no polynomial time algorithms is known. These problems belong to an interesting class of problems, called the NP-Complete problems, whose status is unknown.

In this context, we can categorize the problems as follows −

1.  P-Class Problems
    
2.  NP-Class
    

  

**P-Class Problems**

-   P is a complexity class that represents the set of all decision problems that can be solved in polynomial time, i.e. these problems can be solved in time O(nk) in worst-case, where k is constant.

-   These problems are called tractable, while others are called intractable or superpolynomial.

-   That is, given an instance of the problem, the answer yes or no can be decided in polynomial time.
    
-   Example:

	-   Given a graph connected G, can its vertices be colored using two colors so that no edge is monochromatic?
	    
	-   Algorithm: start with an arbitrary vertex, color it red and all of its neighbors blue and continue. Stop when you run out of vertices or you are forced to make an edge have both of its endpoints be the same color.
    

**NP Class Problems**

NP is the set of decision problems for which there exists a non-deterministic Turing machine that would output a value 1, or 0, after at most O(nk) steps where k is some constant and n is the complexity of the input. We often refer to such number of steps (or running time) as polynomial, hence Non-deterministic Polynomial time, or NP.

  

-   NP = Non-Deterministic polynomial time
    
-   The class NP consists of those problems that are verifiable in polynomial time.
    
-   Verifiable?
    

-   If we are somehow given a ‘certificate’ of a solution we can verify the legitimacy in polynomial time
    
-   Hamiltonian tours (visit every vertex, no vertices can be repeated). NP complete
    
-   Hence, we aren’t asking for a way to find a solution, but only to verify that an alleged solution really is correct.
    
-   Every problem in this class can be solved in exponential time using exhaustive search.
    

# P versus NP problem

-   The P versus NP problem is a major unsolved problem in computer science.
    
-   It asks whether every problem whose solution can be verified in polynomial time can also be solved in polynomial time
    
-   Clearly, P ⊆ NP.
    
-   Arguably the biggest open question in theoretical computer science concerns the relationship between those two classes: Is P equal to NP?
    

-   An answer to the P = NP question would determine whether problems that can be verified in polynomial time, can also be solved in polynomial time.
    
-   If it turned out that P ≠ NP, it would mean that there are problems in NP that are harder to compute than to verify: they could not be solved in polynomial time, but the answer could be verified in polynomial time.
    

**NP Hard**

-   informally, "at least as hard as the hardest problems in NP".
    
-   A decision problem H is NP-hard when for every problem L in NP, there is a polynomial-time reduction from L to H.
    
-   Although it is suspected that there are no polynomial-time algorithms for NP-hard problems, this has not been proven.
    
-   If P ≠ NP, then NP-hard problems cannot be solved in polynomial time.
    
-   NP-hard problems need not be in NP, i.e., they need not have solutions verifiable in polynomial time.
    
-   we may/may-not verify the solution to these problems in polynomial time.
    
-   The following problems are NP-Hard
    
	-   The circuit-satisfiability problem
	    
	-   Vertex Cover
	    
	-   Travelling Salesman Problem
	    


**NP-complete**

-   NP-complete problems are a set of problems to each of which any other NP-problem can be reduced in polynomial time, and whose solution may still be verified in polynomial time.
    
-   That is, any NP problem can be transformed into any of the NP-complete problems.
    
-   A language B is NP-complete if it satisfies two conditions
    
	-   B is in NP
	    
	-   Every A in NP is polynomial time reducible to B.
    

-   If a language satisfies the second property, but not necessarily the first one, the language B is known as NP-Hard.
    
-   Following are some NP-Complete problems, for which no polynomial time algorithm is known.
	-   Determining whether a graph has a Hamiltonian cycle
	-   Determining whether a Boolean formula is satisfiable, etc.
	    
![](https://lh6.googleusercontent.com/7RyeyxJLo6jBhNXbwh-t7gq7XTubzT_K6VobhL9LsgiCg1t5J_HtdsidEbfrhrow799U4VnBpn_zRHVazOLHEvypaMo2l5DgY7J8TXXFK3iv_oj-0tr6Pyeqaf-e1yxLWMrM6PAY)  
  

Prove problem is NP Complete

-   NP-hard problems are also NP-complete
    
-   If an NP-hard problem belongs to set NP, then it is NP-complete
    
-   To belong to set NP, a problem needs to be
    
	-   a decision problem,
	    
	-   the number of solutions to the problem should be finite and each solution should be of polynomial length, and
	    
	-   given a polynomial length solution, we should be able to say whether the answer to the problem is yes/no
    
-   Now, it is easy to see that there could be many NP-hard problems that do not belong to set NP and are harder to solve.
    
-   As an intuitive example, the optimization-version of traveling salesman where we need to find an actual schedule is harder than the decision-version of traveling salesman where we just need to determine whether a schedule with length <= k exists or not.
    

  

![](https://lh3.googleusercontent.com/IxQ6j45ieGAqVwdrxYW8t8nD2bNLNHWjGk872m7E--0-zJH72BkLBvTgiy4GW4mySwffsOIzUmZKxSve8TVMgp9ArqviJZxRPtHrptYxGRlCRfv8a19MzZEFTiFajz7Aym3nxJAB)

  

### NP-Completeness Proof Method

To show that X is NP-Complete:

1.  Show that X is in NP, i.e., a polynomial time verifier exists for X.
    
2.  Pick a suitable known NP-complete problem, S
    
3.  Show a polynomial algorithm to transform an instance of S into an instance of X

	a) Draw the boxes, including inputs and outputs
	b) Write relationship between the inputs
	c) Describe the transformation


**Reducibility**

-   class of “NP-complete” problems has the intriguing property that if any NP-complete problem can be solved in polynomial time, then every problem in NP has a polynomial-time solution, that is, P = NP.
    
-   Despite years of study, though, no polynomial-time algorithm has ever been discovered for any NP-complete problem.
    
-   Intuitively, a problem Q can be reduced to another problem Q’ if any instance of Q can be “easily rephrased” as an instance of Q’, the solution to which provides a solution to the instance of Q.

-   Given an instance ax + b = 0, we transform it to 0X2+ax+b = 0, whose solution provides a solution to ax + b = 0.

  
![](https://lh3.googleusercontent.com/UFohxN5K6aj09yPLNIxK9X88Omlw8wINUgn07_ok9aokI_eIvxyEvdfZNmZjI6I-9e57NK4vfUYg9f1isR30JLGQnj2yGHoJtw-VJAG2X-3NaIlH-ZFwBnNSgqNLq87bHXnKjOIi)![](https://lh5.googleusercontent.com/2LrBZmrmtzuAk5QRmwEHu6BDk7wEZ0d2CAoPEuwThLVkLg--pjq_4zF8nuqZpQJTBYmJ0F1E8yRrZfjHF3x-p4t2Y5QiEWJSTli5Ccl54tKt5xOcelk7Zb0LL9z7q4LoDE3RckCR)![](https://lh3.googleusercontent.com/UPFj_baVHZmwVEloExVv4g6x9igz1XRUesG0kSygpK2vDMBtA3ybPfZiv3A6wpWEfTzYv7hKg9N17pwtI0oylS7fozJd4qEFwt_UIQSYIT-BW-anTdBGcLz1Jg9Y4W90Wn1j-wSy)  
  

Fig: Reducibility

### NP-complete problems

![](https://lh4.googleusercontent.com/m4VJZ5mSh1XpDcS4DC01NPQrndajEBb88BRa2xqnUIDjG-MDhXdZKM5QUu0LegsYWKg8IFTMv2XOpKhuzpaWm6zORe66gvm5nLI094HK2KIfMdS2UDgVpvACGLNmmTttuPSz92C1)

  

Once we prove that at least one problem is NP-complete, we can use polynomial-time reducibility as a tool to prove other problems to be NP-complete. Thus, we now focus on demonstrating the existence of an NP-complete problem: the circuit-satisfiability problem.


### circuit-satisfiability problem

The boolean combinational elements that we use in the circuit-satisfiability problem compute simple boolean functions, and they are known as logic gates.![](https://lh3.googleusercontent.com/b8-rASiz7MwzOxYfF83PpAxDQTdKSiZdpP2fSsQ6d30pOo9Q_BeSnKGzDjr37e-FNK60SNXN6s6hDtd-rRLwEwFb7UJk-1iSUqp6koEPeMiEeT17YnH43-qLsQMSBzv7l7yBdpg7)

  
-   A boolean combinational circuit consists of one or more boolean combinational elements interconnected by wires.
    
-   For the purpose of defining the circuit-satisfiability problem, we limit the number of circuit outputs to 1
    
-   We say that a one-output boolean combinational circuit is satisfiable if it has a satisfying assignment: a truth assignment that causes the output of the circuit to be 1.
    
-   The circuit-satisfiability problem is, “Given a boolean combinational circuit composed of AND, OR, and NOT gates, is it satisfiable?”
    
-   The size of a boolean combinational circuit is the number of boolean combinational elements plus the number of wires in the circuit.
    
-   The circuit-satisfiability problem arises in the area of computer-aided hardware optimization. If a subcircuit always produces 0, that subcircuit is unnecessary; the designer can replace it by a simpler subcircuit that omits all logic gates and provides the constant 0 value as its output. You can see why we would like to have a polynomial-time algorithm for this problem.
    
-   Given a circuit C, we might attempt to determine whether it is satisfiable by simply checking all possible assignments to the inputs. Unfortunately, if the circuit has k inputs, then we would have to check up to 2k possible assignments.
    
-   When the size of C is polynomial in k, checking each one takes average-time-complexity of (2k ) time, which is superpolynomial in the size of the circuit.
    
-   In fact, there is strong evidence that no polynomial-time algorithm exists that solves the circuit satisfiability problem because circuit satisfiability is NP-complete.
    

Boolean satisfiability problem (SAT)

-   SAT is the first problem that was proven to be NP-complete.
    
-   Boolean Satisfiability or simply SAT is the problem of determining if a Boolean formula is satisfiable or unsatisfiable.
    

-   Satisfiable : If the Boolean variables can be assigned values such that the formula turns out to be TRUE, then we say that the formula is satisfiable.
    
-   Unsatisfiable : If it is not possible to assign such values, then we say that the formula is unsatisfiable.

- ![](https://lh3.googleusercontent.com/zu20eaDKMPhBXUJIyyAktUCLrUcNdAT2xnYAMxZYEH2JP48A5BhjadUFGVcr8RWLtTzf9a93Izti7UbiQUoUwsqjI1g44Cq0ptFzLW0YpYMepMbwqt9cs1cBYRslszET8WvqbZUO)
    


Note : Boolean satisfiability(SAT) problem is NP-complete

If there are ‘n’ numbers of Boolean variables viz; x1, x2, x3,....xn, then to find the solution for given boolean expression we need to check 2n  different possibilities, which is an Exponential time algorithm.

**Theorem**

Satisfiability of boolean formulas is NP-complete.

**Proof**

We start by arguing that SAT NP. Then we prove that SAT is NP-hard by showing that CIRCUIT-SAT pSAT
  
-   To show that SAT belongs to NP, we show that a certificate consisting of a satisfying assignment for an input formula can be verified in polynomial time.
    
-   The verifying algorithm simply replaces each variable in the formula with its corresponding value and then evaluates the expression.
    
-   This task is easy to do in polynomial time. If the expression evaluates to 1, then the algorithm has verified that the formula is satisfiable. Thus, the first condition SAT NP is satisfied. 

-   To prove that SAT is NP-hard, we show that CIRCUIT-SAT pSAT. In other words, we need to show how to reduce any instance of circuit satisfiability to an instance of formula satisfiability in polynomial time.
    

-   We can obtain the formula for the circuit by writing an expression that applies the gate’s function to its inputs.
    
-   Given a circuit C, it is straightforward to produce such a formula in polynomial time.
    
-   if some assignment causes formula to evaluate to 1, the circuit is satisfiable.
    
-   Thus, we have shown that CIRCUIT-SATpSAT, which completes the proof.
    

  
  
  
  
  
  
  

### 3-CNF Satisfiability(3-SAT or 3-CNF-SAT)

  

what is Conjunctive Normal Form (CNF) or also known as Product of Sums (POS).

CNF : CNF is a conjunction (AND) of clauses, where every clause is a disjunction (OR).

Now, 2-SAT limits the problem of SAT to only those Boolean formula which are expressed as a CNF with every clause having only 2 terms(also called 2-CNF).

Example:

![](https://lh6.googleusercontent.com/NuhPsmHnR3FtOyz1kaPNdpkYmUnB3Mcd4IDIlIm9DsD8VrrO7peSUclbqpX-rh6Vc4NceDFBhl3HvNybGKlRuKTAblr2Pz_19kNC8MQFMKntwhjWfikEnA3EpiWKffFL7mH3fpUT)

  
Similarly, 3-SAT limits the problem of SAT to only those Boolean formula which are expressed as a CNF with every clause having only 3 terms(also called 3-CNF).

 
Example:

  
![](https://lh3.googleusercontent.com/7WQghgnvVUBKHVv8ZwGIcgHF1VEsBO9-b0XXEcizO_6RtsCmIjqq1b6ciBbegeYw1HLSUy3lBLc9qIF8TNqJTCiLCnawme7Ev_6cKptfq0ubfX6Fz-kyHm6eITJ21Sa5sx-9k4tW)

Thus, Problem of 3-Satisfiability can be stated as:

-   Given CNF with each clause having only 2 terms, is it possible to assign such values to the variables so that the CNF is TRUE?
    
-   Formally,
    

3-CNF-SAT = { <ψ>:ψ is a satisfiable 3CNF }

For the CNF value to come TRUE, value of every clause should be TRUE.

Example:

Let one of the clause be A V B. To get evaluate this clause to be TRUE,

-   If A = 0, B must be 1 i.e. ![](https://lh5.googleusercontent.com/RkJk9775KGL_69gBoDBxnPDPZKUqPQRriyXtj26pAyqT1XKmHsEQGDUGRCjhflSPrPIqYr4Rtcx63DNH9R821WzdDXOeLU30nlGxKWClRuL-cW8BpkWXDAp7tgW_EzpqOT0TYEtz)
    
-   If B = 0, A must be 1 i.e. ![](https://lh5.googleusercontent.com/JXUolETee7jT8QLTeGMkFc3sNLf2MPenoEshmVbkEReEQfVNSmKbINhnTBn-MI3hAoJLg4znmD4c-7GBPyLMfznSqnhLOnkjLCxXn7PPhLWXpfisO5LQwt8nqgs_Fb0XLgKYctKB)
    

**Theorem**

Satisfiability of boolean formulas in 3-conjunctive normal form is NP-complete.

Proof

-   The argument we used in the proof of to show that SATNP applies equally well here to show that 3-CNF-SATNP.
    
-   Therefore, we need only show that SAT p3-CNF-SAT
    
-   We break the reduction algorithm into three basic steps. Each step progressively transforms the input formula closer to the desired 3-conjunctive normal form.

Step 1:

Construct a binary parse tree for the input formula , with literals as leaves and connectives as internal nodes.

![](https://lh6.googleusercontent.com/P7P2kCzqDEbXoJoZeG_N3kJnI-rp1j_UTpRR7X5jC5z9pZq3aPpVPDxcfWCuWkVnO4gzTl6uVXa2xnT0eQnBehQCp9YpwTe4QtwpnZEeQVE6ExZmTr5S7bLy_d4fTObxlty--MMQ)

  
![](https://lh6.googleusercontent.com/FWrkE5RS_C11imbopEo7-ig7nA-Vv54bhaTk6Oz_TpC2VODqsBSTYZ5ob904q9TorNiKYcEo-mfYQOJiBFXx4gDwt6lQaCyT_-Bznxxo0HlNuQ41K9lxYlIium5d62hAZhSMc35z)  
 
  

Step 2:

The second step of the reduction converts each clauseinto conjunctive normal form.

Step 3:

The third and final step of the reduction further transforms the formula so that each clause has exactly 3 distinct literals. We construct the final 3-CNF formula ’’ from the clauses of the CNF formula ’.

-   The resulting formula after this conversion is equivalent to .
    
-   We can see that the 3-CNF formula ’ is satisfiable if and only if is satisfiable by inspecting each of the three steps.
    
-   Besides, all the reduction steps can be done in polynomial time
    
-   Thus we have successfully proved that SAT p3-CNF-SAT. And as SATNP; 3-CNF-SATNP
    
### Clique Problem

  
![](https://lh6.googleusercontent.com/kpMfcPcmVaQriNpRTgg8kpac4Lwsd1fWqQuE5ev2huiP8AScOmsV8MJ9qUXC2uc_b40-nuwiFoyc_6wImG0_IaOEf2ch3dEgEwmmaQVMpqajJguLc7IG3c-mCfia8rWa-XrFJU9h)  

In the mathematical area of graph theory, a clique is a subset of vertices of an undirected graph such that every two distinct vertices in the clique are adjacent; that is, its induced subgraph is complete.

  
  
  

-   Definition: A clique in an undirected graph G=(V, E) is a subset of vertices, each pair of which is connected by an edge in E, i.e., a clique is a complete subgraph of G.
    ![](https://lh6.googleusercontent.com/QZ7dLtWJWGaAYRRO0RinFDV6KLk09rC5fVMHyYiqUsKJUtP8x2Yi7IW-na1kO-oKJPnLP1I1Jb7qhSMGLcTetg_dn0Gch3FML5PbNWpDYMer1xZR5YsInbzwq9BmeRzVB_tW8t8v)
-   A clique is a subset of vertices that are all connected
    
-   Size of a clique is the number of vertices in the clique.
    
-   In computer science, the clique problem is the computational problem of finding cliques in a graph.
    

-   Optimization problem: find the maximum clique value in given Graph.
    
-   Decision problem:  whether a clique of given size k exists in the graph?
    

-   CLIQUE={<G,k>: G is a graph with a clique of size k.}
    

  
  
![](https://lh3.googleusercontent.com/fHB0ME_KQ59KQxrBIbV_QYuh_SRv8k68sOwIxxk4NUUWQWZeXVMsGA32PRCFncp-i8ab5twaZ6eQr_l72WS6wbyiHmGtJii_wqS1WA4iTjOEDLb8lkXZz-smLNc27_JgdHTyzJeZ)![](https://lh4.googleusercontent.com/6PhkAm7aidSkfQieTMoJr2uTCWgB6R-3ePJ8RL-w5c_5-yYeuvgac5cJdIe2j3Ha7G42tCc3ch5YxMdvzK2GqE1hA7vFugAb-sDHX8N8vR4wRhL2aptZN-npMLX29FuhaUErFn6n)  
  
  

-   Clique belongs to NP
    
-   Any NPC problem can be reduced to clique problem
    
![](https://lh5.googleusercontent.com/2xi4QdxAg52S1bY7aFev7xCvfJdQY7njHyZRgaqEzKvgRXQ3NwHPQh_YoEKsspMqDxp9w6UsNoqFU5f2AbBK8pdQBdL6xSVeIFiKT3nNZ-9BeMd8gWbV_gdbBUFBijow1fGws5-z)

  
  ---

**Theorem**

The clique problem is NP-complete.


Proof

-   To show that CLIQUE NP, The verification algorithm takes G=(V, E) and a subset V’ of V vertices as the certificate.
    
-   We can check whether V’ is a clique in polynomial time by checking whether, for each pair u,v V’, the edge (u,v)E.
    
-   We next prove that 3-CNF-SATpCLIQUE, which shows that the clique problem is NP-hard.

-   The reduction algorithm begins with an instance of 3-CNF-SAT.
    
-   Let =C1 ^ C2 ^ ... ^ Ck be a boolean formula in 3-CNF with k clauses.
    
-   For r = 1,2,3,...,k, each clause Cr has exactly three distinct literals l1r,l2r and l3r
    
-   If there are k clauses, we ask whether the graph has k cliques. Such cliques exists if and only if there is satisfying assignment. Conversely is satisfiable if and only if G has a clique of size k.
    
  
![](https://lh6.googleusercontent.com/eJ9qOnRKxqFUo54n9m_GeDTBrKT7EKBTjeh-6itzxXW3rD8QaJGDI-xUoHRGx_MslqJaMtY91lqziXScvKwmRu3jOJfKm5dyYp-MPHmERcYXEcvdtcUKVYWS_SaQY9RoPocY81jR)![](https://lh4.googleusercontent.com/mrW_vlztPDgWzqmi5dDk9RRW6Hw_4g7_8KJXH6-9hATtJ4cLeAZ7zItehs0-lyOTDzhGdHIaLRI3ePSZS-c0TWi-VdJ5EXFPPZ6A__9WmUehKreYj6zfCGZX9zwcxUtDu5ILQFNS)

Vertex cover problem

  
![](https://lh5.googleusercontent.com/mC5Jb6symWdRhDZRLaQ7Ln_SsSoTd0ryxo8VvDEZQiKvcGre42Rco6R7vrGwB7ZM_1V2W9NKxyvNhJx9g43aaxnlnmsyYrKTDl6Khqt75pUFdRAGnQxk4-BbFwi_FqeQgx24-D7X)  

-   In the mathematical discipline of graph theory, a vertex cover (sometimes node cover) of a graph is a set of vertices such that each edge of the graph is incident to at least one vertex of the set.
    
-   The problem of finding a minimum vertex cover is a classical optimization problem in computer science and is a typical example of an NP-hard optimization problem that has an approximation algorithm.
    
-   Its decision version, the vertex cover problem is one of a classical NP-complete problem in computational complexity theory.
    
-   A vertex cover of an undirected graph G = (V, E) is a subset V’ V such that if (u,v)E, then u V’ or v V’ (or both).
    
-   That is, each vertex “covers” its incident edges, and a vertex cover for G is a set of vertices that covers all the edges in E.
    
-   The size of a vertex cover is the number of vertices in it.
    
-   The following figure shows two examples of vertex covers, with some vertex cover V’ marked in red.![Vertex-cover.svg](https://lh3.googleusercontent.com/9-nLKq27zlj9HKB3jx0RSVyMWFfj7KxxiYfq1UlOwxMyAaiIXU97Z_tAfk5lXIkqvKbkyRKkmGDQeLzn3zzRT3bquXk6zi9dM5CytrmcIjTW69ItO1wVRmsC71yxhC62BnQ-17l4)
    
The minimum vertex cover problem is the optimization problem of finding a smallest vertex cover in a given graph.

INSTANCE: Graph G

OUTPUT: Smallest number k such that G has a vertex cover of size k.

  
![Minimum-vertex-cover.svg](https://lh5.googleusercontent.com/egxlmz9-jyI_rj22zENVN9JN2yzOszpsRrVj-hqJJL7U_8B-wSWG4jBfxsHe2HDE56J58oBPxOujawCyQTdOEDqnwuTNOw8YpeTaJ3Xb8X9eOa2oLsPgQn1CXERe94Ytab8wfHi0)

  

Restating this optimization problem as a decision problem, we wish to determine whether a graph has a vertex cover of a given size k.


If the minimum vertex cover problem is stated as a decision problem, it is called the vertex cover problem:

INSTANCE: Graph G and positive integer k.

QUESTION: Does G have a vertex cover of size at most k?


As a language, we define

-   VERTEX-COVER ={ (G, K): graph G has a vertex cover of size k}
    

  

The following theorem shows that this problem is NP-complete.

**Theorem:**

The vertex-cover problem is NP-complete.

Proof

-   We first show that VERTEX-COVER ∈ NP.
    

-   Suppose we are given a graph G = (V, E) and an integer k.
    
-   The certificate we choose is the vertex cover V’ V itself.
    
-   The verification algorithm affirms that |V’| = k, and then it checks, for each edge (u,v)E,, that u ∈ V’ or v ∈ V’.
    
-   We can easily verify the certificate in polynomial time.
    

-   We prove that the vertex-cover problem is NP-hard by showing that CLIQUE pVERTEX-COVER.
    

-   This reduction relies on the notion of the “complement” of a graph.
    
-   Given an undirected graph G=(V, E), we define the complement of G as G= (V, E), where E = {(u,v): u, v ∈ V, u v, and (u,v) E}.
    
-   In other words, G is the graph containing exactly those edges that are not in G.
    
-   The reduction algorithm takes as input an instance (G,K) of the clique problem.
    
-   It computes the complement G, which we can easily do in polynomial time.
    
-   The output of the reduction algorithm is the instance (G, |V| - k) of the vertex-cover problem.
    
![](https://lh3.googleusercontent.com/JgqIBRFcIFQ7Ma5jDsC3MvhC-7qdeWmXqwQeXtzrNsRzNLYueBxtJeLIlQjQWUe40Me4-ph_su5S3Bay2RFUgOlro2FYQZoXojD-Cs453rBKE5l6DQYF6B5W2QxYuZmxk4heAjaC)

-   To complete the proof, that this transformation is indeed a reduction: the graph G has a clique of size k if and only if the graph G has a vertex cover of size |V| - k.
    

For example in above fig

-   The original graph has a u,v,x,y CLIQUE. That is a clique of size 4.
    
-   The complement graph has a vertex cover of size 6 (number of vertices) – 4 (clique size) = 2. z,w is one such vertex cover.
    
---

Summary:

  

-   (Total number of vertices in graph) - (Independent Set in graph) = Vertex Cover.
    
-   The vertex cover problem gives you the smallest number of vertices that cover the most number of edges. Now to understand how complimenting a graph will help us here, let's quickly see two other problems.
    

-   1. The clique problem seen gives us the largest number of vertices that are fully connected (a subgraph that is also complete). That means they have the largest number of edges between them.
    
-   2. The independent set problem gives us the largest number of vertices with no edges between them. This means that they have the largest number of vertices that are not connected.
    
-   Now if you take a graph and complement it by removing all current edges and creating edges between vertices that were not originally connected, you can find the clique of this new complimented graph and it will give you the independent set of the original graph. Why? Because the clique of a graph where all unconnected vertices are now connected will give you a set of vertices that were previously independent (or strangers).
    
-   Now suppose. Let's say you have a graph G and you find the clique of this graph and name it C. Let's also name the remaining vertices in graph G that are not in clique C, we'll name them ~C. Let's now take the complement of graph G and name it ~G. The clique of graph G is the independent set of graph ~G, while the clique of graph ~G is the independent set of G. Interestingly enough, the remaining vertices in ~C for graph G form the vertex cover of ~G. And the remaining vertices in ~C for graph ~G form the vertex cover for graph G. Try it out in a small graph and you'll see.
    
---
  

### The hamiltonian-cycle problem

  

-   Formally, a hamiltonian cycle of an undirected graph G = (V, E) is a simple cycle that contains each vertex in V .
    
-   A graph that contains a hamiltonian cycle is said to be hamiltonian; otherwise, it is non-hamiltonian.
    
-   In the mathematical field of graph theory, a Hamiltonian path (or traceable path) is a path in an undirected or directed graph that visits each vertex exactly once. A Hamiltonian cycle (or Hamiltonian circuit) is a Hamiltonian path that is a cycle.
    
-   One possible Hamiltonian cycle through every vertex of a dodecahedron is shown in red, the dodecahedron is Hamiltonian
    

![enter image description here](https://lh3.googleusercontent.com/X-L5HM-aBO2k5V8oeUFO1uAp2EtCJXrezR3opgw-NbMidBUX2wPItqPm3lTH09XYXvHG23X2kSiMIlwyB7Z88_w5zjA1mrvHVBFMGypc73lqjPOuWZNo2bcYz16iMrgybRVnu9mS)

![](https://lh4.googleusercontent.com/fgMy2-SW8wuXgk7wHxj1WI5hVZjLL4bMyWYeRsz1SO1XVXycBleLUSBOs8hO061h2KoZ4GayvwAEr-WCULSE0mypXHttyU2IeZ_Vrwhq8kqD-wNBpaD1zcBbPqdSnKBnZKpLY89g)  ![](https://lh5.googleusercontent.com/1YoPGSuCNNGH_bcCnji57D5mW5MUoXahyoIdkTxyKX1Qau6p0n15Q-_qjH_5BYxEGlRMRau2AdA1Yd6614IdNKuXb8FzOuLMZKCN6wfXgT_fYmj9kePnGtYqNzI9QjgMNBiKk7fI)

  
  

-   hamiltonian-cycle problem:
    

-   “Does a graph G have a hamiltonian cycle?”
    
-   as a formal language:
    

-   HAM-CYCLE = {(G) : G is a hamiltonian graph}.
    

  
  

Theorem

The hamiltonian cycle problem is NP-complete.

  

Proof

-   We first show that HAM-CYCLE belongs to NP.
    

-   The verification algorithm takes G=(V, E) as the input and a list of vertices that makes up the hamiltonian cycle, as the certificate.
    
-   The verification algorithm checks that this sequence contains each vertex in V exactly once and there is an edge between each pair of consecutive vertices and between the first and last vertices such that it forms a cycle in G.
    
-   We can verify the certificate in polynomial time.
    

  

-   We now prove that VERTEX-COVER p HAM-CYCLE, which shows that HAM-CYCLE is NP-complete.
    
----
 
## The traveling-salesman problem

-   The Travelling Salesman Problem describes a salesman who must travel between N cities.
    
-   The order in which he does so is something he does not care about, as long as he visits each once during his trip, and finishes where he was at first.
    
-   There is an integer cost c(i, j) to travel from city i to city j, and the salesman wishes to make the tour whose total cost is minimum, where the total cost is the sum of the individual costs along the edges of the tour.
    
-   Modeling the problem as a complete graph with n vertices, we can say that the salesman wishes to make a tour, or hamiltonian cycle, visiting each city exactly once and finishing at the city he starts from.
    

Theorem

The traveling-salesman problem is NP-complete.

  

Proof

-   We first show that TSP belongs to NP.
    

-   Given an instance of the problem, we use as a certificate the sequence of n vertices in the tour.
    
-   The verification algorithm checks that this sequence contains each vertex exactly once, sums up the edge costs, and checks whether the sum is at most k.
    
-   This process can certainly be done in polynomial time.
    

-   To prove that TSP is NP-hard, we show that HAM-CYCLE pTSP.
    

-   Let G = (V, E) be an instance of HAM-CYCLE.
    
-   We construct an instance of TSP as follows.
    

-   We form the complete graph G'= (V,E'), where  E' = {(i, j): i, j  ![](https://lh3.googleusercontent.com/w0UXhaIhJ71vo5i_3T_nRLeC_YlUHzX53wG_HgQmdJTkDqyJ2YOaBLcWTpqp1vc2S8ZwwklUKmxxK38F8snxCKp_Ikz2STxcrEyXWSQmhb5e58KEyJhgwtu7rmJtaBI4OUl9qqSR)  V}, and we define the cost function c by![](https://lh5.googleusercontent.com/0eBL1z7QpqCStgTJzm3x0jhlAeyeSRZtLg4YiZOGOuoIzqA2rGXFEC_y8XGYV9PCCuARvtDfxWX32aaLKxwbUZRVsXnIcqg0GTW-_vwYpTHnB_KdmaHWXep_38X4Y_UiyQCCMB0D)
    
-   The instance of TSP is then (G', c, 0), which is easily formed in polynomial time.
    

  
![](https://lh5.googleusercontent.com/PcDzKignSWCBIxFCF3NhpzzP6mlleajs4MJbpNjRw6bqaGt3AikBzWPSbSRb3MDZuOtMU0gsFHDydQQSY8-3KJ9y9P40HYDLbEDOUeLwGJpWaouQuQpoYV7R2xq9FNKUUHcH4Gya)  
  
 
-   We now show that graph G has a hamiltonian cycle if and only if graph G' has a tour of cost at most 0.
    
-   Suppose that graph G has a hamiltonian cycle h. Each edge in h belongs to E and thus has cost 0 in G'. Thus, h is a tour in G' with cost 0.
    
-   Conversely, suppose that graph G' has a tour h' of cost at most 0. Since the costs of the edges in E' are 0 and 1, the cost of tour h' is exactly 0. Therefore, h' contains only edges in E.
    
-   We have thus proven that G has a Hamiltonian cycle, if and only if G' has a tour of cost at most 0.
    
-   And thus TSP is NP-complete.
