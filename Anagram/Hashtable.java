import java.util.Arrays;
import java.util.LinkedList;
public class Hashtable {
LinkedList<MyLinkedList>[] array; //The hashtable is an array of linked lists of linked lists 
static int size; //Size of the hashtable
static int[] prime = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101};
//First 26 primes, used in the hash function where each letter is represented
//by a prime


public Hashtable(int size){ //constructs a hash table of the given size
Hashtable.size = size;
array = new LinkedList[size];
for(int i = size-1;i>=0;i--) //initializes each position of the array
	array[i] = new LinkedList<MyLinkedList>();
}

public static int calcVal(String s){ //calculates the hash value

	char[] temp = s.toCharArray();
	int total = 1;
	for(int i= 0; i<temp.length; i++) //runs through each character
		total = Math.floorMod(total*getVal(temp[i]), size);
	return total; //product of the corresponding primes for each character
}

public MyLinkedList insertHelp(String s, int val){ //finds where to place 
	char[] c = s.toCharArray();					   //the word
	int count = 0;
	int[] freq = new int[26]; //frequency table for the word being added
	MyLinkedList cur = new MyLinkedList();
	for(char a: c){
		freq[a-97]++;
	}
	if(!array[val].isEmpty()){ //if something is already hashed to this location,
		cur = array[val].getFirst();	//search for the correct anagram class
		while(cur!=null){
			if(Arrays.equals(cur.freqTable, freq)){
				return cur; //return the linked list the word belongs to
			}	
			else{
				count++;
				if(count>=array[val].size()){
					cur = new MyLinkedList(); //if never found a matching anagram,
					break;					  //make a new anagram linked list
				}
				cur = array[val].get(count); //proceed to next anagram class 
			}
		}
	}
	cur.freqTable = freq; //if it is a new MyLinkedList, will now have the frequency
	return cur; // of the added word, otherwise the frequency tables are already equal
}
public void insert(String s){ //places a word into the correct anagram class 
	if(s.length()<1) //prevents empty strings
		return;
	int val = calcVal(s);
	MyLinkedList pos = insertHelp(s,val);
	if(pos.list.size() == 0){ //if the word doesn't match an anagram class make a new one
		pos.list.add(s);
		array[val].add(pos);
	}
	else{ //if the anagram class the word fits into already exists, add to that
		pos.list.add(s);
	}
}

public static int getVal(char c){ //turns the given char into a prime
int temp = c-97;
return(prime[temp]);
}
 


}
