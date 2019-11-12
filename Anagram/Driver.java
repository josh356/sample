import java.io.*;
public class Driver {
public static void main(String[] args){
	int size = 1000000;
	Hashtable x = new Hashtable(size);
	
	String file = args[0]; //takes in an input file
	String fileWrite = args[1]; //and an output file
	
try {
	FileReader fileReader = new FileReader(file);
	BufferedReader bufReader = new BufferedReader(fileReader);
	FileWriter fw = new FileWriter(fileWrite);
	BufferedWriter bw = new BufferedWriter(fw);
	String line;
	while(bufReader.ready()){ //reads and then inserts each line of the input into the hashtable
		line = bufReader.readLine();
		x.insert(line); //inserts each word
		}
	
	//run through the hashtable and its linked lists
	//printing to the output file each anagram class
	for(int i = 0;i<size;i++){  //each linked list of the array 
		for(int j = 0; j< x.array[i].size(); j++){ //each anagram class of the linkedlist
			if(x.array[i].get(j).list.size()>0){
				for(int k = 0; k < x.array[i].get(j).list.size();k++){//each word of the anagram class
					bw.write(x.array[i].get(j).list.get(k)+" ");
					bw.flush();
				}
			bw.write("\n");
			}	
		}
	}
	bufReader.close();
	bw.close();
} 
catch (FileNotFoundException e) {
	e.printStackTrace();
} 
catch (IOException e) {
	e.printStackTrace();
}

}
}
