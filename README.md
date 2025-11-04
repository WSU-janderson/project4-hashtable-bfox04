[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/7RAFczHS)
# Project 4 - HashTable

Project description can be found [in the repo](Project4_HashTable.pdf)

Place your complexity analysis below.

---

[Where alpha = size / capacity]

Average Case: O(1/(1-alpha)) , Worst Case: O(N)

    insert:

        The average case is O(1/(1-alpha)) because resize() keeps the load factor (alpha) <= 0.5, meaning we expect to hit an open slot in just a few probes.

        The worst case is O(N) because, if every key collides, we might have to search almost all N buckets along the randomized probe sequence before finding an empty spot.

    remove:

        The O(1/(1-alpha) average case applies because, with a low alpha, usually find the key to be removed very quickly within the first few checks of the probe sequence.

        It is O(N) in the worst case because a chain of collisions means we might have to check every bucket in the table to confirm the key is either not present or is the last element in that chain.

    contains:

        In the average case, O(1/(1-alpha)), we expect to find the key quickly because low load means short probe sequences and few collisions.

        The worst-case O(N) happens if the key maps to a completely full collision sequence, forcing us to check up to N buckets until an Empty Since Start (ESS) bucket terminates the search.

    get:

        The average time is O(1/(1-alpha)) because the search is highly efficient with a low load factor, quickly leading us to the key's value.

        It's O(N) worst-case since we might have to scan the entire table's probe sequence before we finally find the key or hit an ESS bucket.

    operator:

        This function's time is dominated by the search for the key, so the average case is O(1/(1-alpha)) due to the table's low load factor and efficient probing.

        The worst-case O(N) is a result of a collision chain that forces the search to iterate through most of the N buckets before finding the key or returning the undefined behavior.
