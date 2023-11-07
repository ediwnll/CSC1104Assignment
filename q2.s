// bubblesort.s in ARM/Aarch64 assembly language for execution on Raspberry Pi 4 Model B
//
// argument registers (x0-x7)
// x1/w1 = no. of passes to loop through the array and swap elements (equal to no. of elements in array) &&  also to store values in preparation for printf function
// x2 = bubblesort functions pointer to the "array" in the memory address (value is a memory address)
// w3 = bubblesort functions outer loop iteration variable for no. of passes bubblesort has to go through 
// w4 = bubblesort functions inner loop iteration variable for the current iteration of array element
// w5 = 1st register to load/store values from the memory address for comparing & swapping
// w6 = 2nd register to load/store values from the memory address for comparing & swapping
// callee-saved registers (x19-x28)
// x19 = memory address [sp,4] for which the arrays we will be using will be stored at  
// x20 = loop iteration variable for: (the loop generating psuedo-random numbers) & (the loop to print the sorted arrays elements)
// x21 = system timer register (cntvct_el0) value before the .bubblesort function
// x22 = time taken to execute bubblesort function 
// 
// x24 = virtual page address of .printArrayElement, saved to register since used >1 time
// 

	.text
	.align 2
	.global	bubblesort
bubblesort:				// start of bubblesort function; for x no. of elements generated & stored in an array
.bsStart:				
// initialises variables used in other parts of the bubblesort outer loop
	sub	w1, w1, #1		// (outer) pass loop: initialise total no. of elements to sort as total no. of passes to iterate through, 
						// w1 = array length, w1 = w1-1 (zero indexing)
	mov	w3, 0			// (outer) pass loop: initialise pass iteration, w3 = 0
.bsOuterCMP:
	cmp	w1, w3			// (outer) pass loop: compares the pass iteration (w3) & array length (w1)
	bgt	.bsInnerStart	// if array length (w1) > pass iteration (w3), branch to .bsInnerStart 
	ret					// Otherwise, return to main 
.bsInnerStart:			
// initialises variables used in the inner loop, including the  
// (iteration variable for the current no. of the array element) & (the pointer to the memory address for the array element). 
	add	x2, x0, 4		// x2 is the functions pointer to the current array element (every loop: +4 byte offset points = next address in memory). 
						// It is initialised as the memory address after x0. x0 is the functions reference to where the array is stored. 
						// DO NOT use x0 directly as it will lead to a segmentation fault (because as .bsInnerStart is iterated through again, x0 will change)
	mov	w4, 0			// Initialises inner loop iteration variable, w4 = 0 (distinct from x2, w4 only keeps count of no. of loops)
.bsInnerSwap:				// 
	ldp	w5, w6, [x2, -4]	// Loads two words (32-bit) (arr[i] & arr[i+1]) from [x2, -4] & [x2], & writes them to registers w5 & w6   
	add	w4, w4, 1			// increment inner loop variable (w4+1) 
	cmp	w5, w6				// compares the loaded register values, w5 & w6
	ble	.bsInnerArrPtrInc	// if w5 <= w6, branch to .bsInnerArrPtrInc to continue going through the array
	stp	w6, w5, [x2, -4]	// Otherwise, store the two words from registers w5 & w6 in REVERSE order (w6, w5) back to the 
							// memory address [x2, -4] & [x2] (values in the array are swapped)
.bsInnerArrPtrInc:			// 
	add	x2, x2, 4			// updates pointer x2 to the next iteration/element/memory address in the array
	cmp	w1, w4				// compares array length (w1) & inner loop iteration variable (w4)
	bgt	.bsInnerSwap		// if w1>w4, branch to .bsInnerSwap to continue comparing & swapping elements in the array
	add	w3, w3, 1			// Otherwise, when done iterating through all elements in the array -> increment pass iteration (w3)
	b	.bsOuterCMP			// branch to (outer) pass loop (.bsOuterCMP) to repeat until w3=w1

.printArrayElement:
	.asciz	"\n%d"
.stringForSortedArr:
	.asciz	"\n The sorted list is as follows:"
.stringExecTime:
	.asciz	"\n Time taken to sort the array using bubble sort algorithm is %d nanoseconds\n"
	
	
	.align	2		//aligns addresses to 4-bytes, we ran into bus errors without this alignment
	.global	main
main:
.mainStart:
	add	x19, sp, 4	// x19 is the memory address, [sp,4], of the arrays which we will be using, 
					// we cannot use [sp,0] as it will result in the program to hang
	mov	x20, 0		// initialise loop iteration variable = 0, (for the loop generating psuedo-random numbers) & 
					// (for the loop to print the sorted arrays elements)


.loopPrintRNG:					// loop of the external function, rand, to generate values to store in the array
	bl	rand					// branch link to rand function to generate psuedo-random numbers and store it at w0
	mov	w1, w0					// store the generated no. at w1 for printing as %d
	str	w0, [x19, x20, lsl 2]	// store the generated no., w0 at memory address [x19+(x20*4)]. 
	// So, at iteration 0, it stores the value at x19, [sp,4]. At iteration 1, it stores the value at x19+(iteration no. *4), [sp,8]

	adrp	x24, .printArrayElement			// x24 = calculated virtual page no. of .printArrayElement (upper 52 bits)
	add	x24, x24, :lo12:.printArrayElement	// adds lower 12bits of .printArrayElement to complete its 64-bit virtual page address 
	mov	x0, x24							// move virtual page address of .printArrayElement to x0, preparing it for printf function
	bl	printf							// print .printArrayElement ("\n%d") (most recently generated pseudo-random number) 

	add	x20, x20, 1		// increment count for rand number generation loop
	cmp	x20, 202		// compare loop iteration to the total no. of elements to generate
	bne	.loopPrintRNG	// if less than the total no. of elements, branch to .loopPrintRNG 
						// to loop generating & printing the pseudo-random no. 

	mov	x1, x20			// x1 = total no. of elements to sort in bubblesort function (note that x20 has incremented to a value of 202). 
						// We use w1 in .bubblesort instead as its a small value.
	mov	x0, x19			// x0 = memory address for arrays, so .bubblesort can store the sorted values in the memory addresses offset from x0

	mrs x21, cntvct_el0	// read system timer register & store it at x21
	bl	bubblesort		// branch link to bubblesort function, 
						// uses x19 (memory address which arrays are stored at) & x1 (no. of passes to complete)
	mrs x22, cntvct_el0	// read system timer register after the bubblesort & store it at x22
	sub x22, x22, x21	// x22 = time to execute bubblesort function  

										
	adrp	x0, .stringForSortedArr			// x0 = calculated virtual page no. of .stringForSortedArr (upper 52 bits)
	add	x0, x0, :lo12:.stringForSortedArr	// adds lower 12bits of .stringForSortedArr to complete its 64-bit virtual page address 
											// to prepare x0 for printf function
	bl	printf								// branch link to printf function ("\n The sorted list is as follows:")

	mov	x20, 0		// re-initialise x20 = 0 to loop through & print the bubblesort-ed elements in the array


.loopPrintSorted:
	ldr	w1, [x19, x20, lsl 2]	// load elements from bubblesort-ed array into w1.  
								// For every loop iteration increment, x20+1, it loads the next value from the array.
	mov	x0, x24					// move virtual page address of .printArrayElement to x0, preparing it for printf function
	bl	printf					// print .printArrayElement ("\n%d") (prints the current iterated element of 
								// the bubblesort-ed array, printed in ascending order) 

	add	x20, x20, 1				// increment loop count for printing the sorted array
	cmp	x20, 202				// compare loop iteration to the total no. of elements in the array
	bne	.loopPrintSorted		// if less than the total no. of elements, branch to .loopPrintSorted to loop 
								// loading & printing the elements from the bubblesort-ed array
										
	mov x1, x22							// store the time to execute the bubblesort at w1 for printing as %d 
	adrp	x0, .stringExecTime			// x0 = calculated virtual page no. of .stringExecTime (upper 52 bits) 
	add	x0, x0, :lo12:.stringExecTime	// adds lower 12bits of .stringExecTime to complete its 64-bit virtual page address 
										// to prepare x0 for printf function
	bl	printf							// branch link to printf function ("\n Time taken to sort the array using bubble sort algorithm is %d nanoseconds\n")

	ret									// return to the address stored in x30
