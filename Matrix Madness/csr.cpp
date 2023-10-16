/*
  auth: Aisosa Aimufua 
  date: 9/22/23
  desc: full implemenation of the csr/csrlist class
*/
#include "csr.h"

///////////////CSR Class Implementation///////////////

// v and col_val will be the same size
// col_index and row_index will both be dynamically allocated arrays
CSR::CSR(){
    m_values = nullptr;      //array to store non-zero values
    m_col_index = nullptr;   //array to store column indices
    m_row_index = nullptr;   //array to store row indices 
    m_nonzeros = 0;     //number of non-zero values
    m_m = 0;            //number of rows
    m_n = 0;            //number of columns
    m_next = nullptr;        //pointer to the next CSR object in linked list
    
}
CSR::~CSR(){
    clear();

}
CSR::CSR(const CSR & rhs){
    // set the members to the rhs
    m_nonzeros = rhs.m_nonzeros;
    m_m = rhs.m_m;
    m_n = rhs.m_n;

    // dynamically allocate memory for the arrays that'll be copied over to the rhs
    m_values = new int[m_nonzeros];
    m_col_index = new int[m_nonzeros];
    m_row_index = new int[m_m + 1];


    // Populating the arrays with actual values
    for (int i = 0; i < m_nonzeros; i++) {
        m_values[i] = rhs.m_values[i];
        m_col_index[i] = rhs.m_col_index[i];
    }

    for (int j = 0; j <= m_m; j++) {
        m_row_index[j] = rhs.m_row_index[j];
    }

    // We don't need copy m_next here because its implied that copy constructor is copying/making
    // a standalone CSR object, thats not connected to any list so we can just set it to nullptr.
    m_next = nullptr;
}

void CSR::clear(){

    delete[] m_values;

    delete[] m_col_index;

    delete[] m_row_index;
    m_m = 0;
    m_n = 0;


    // If you allocated a single object with new, use delete. 
    // If you allocated an array of objects with new[], use delete[]
    m_col_index = nullptr;
    m_row_index = nullptr;
    m_values = nullptr;
    m_next = nullptr;
    
}
bool CSR::empty() const{
    return m_m == 0 and m_n == 0;
}

void CSR::compress(int m, int n, int array[], int arraySize){
    // check for the 0x0 matrix case
    if (m == 0 and n == 0) {
        // Initialize the CSR object as an empty object
        return;
    }
    // First we want to keep track of the number of non-zero elements in the matrix being passed and values
    int non_zero_count = 0;
    int valuesIndex = 0;
    for (int i = 0; i < m * n; i++) {
        // Get value from array or default to 0
        int currentValue = (i < arraySize) ? array[i] : 0; 
        if (currentValue != 0) {
            non_zero_count++;
        }
    }

    // If the array/matrix already has memory allocated (basicaly it's being reused/called again)
    // we wanna clean up and deallocate that memory
    if (m_col_index != nullptr) {
        delete[] m_col_index;
    }

    if (m_row_index != nullptr) {
        delete[] m_row_index;
    }

    if (m_values != nullptr) {
        delete[] m_values;
    }

    // Else if its not being reused we can dynamically allocate memory for the value and column arrays
    m_col_index = new int[non_zero_count];
    m_values = new int[non_zero_count];

    // Next we allocate memory for the row array of the size that we pass to it +1
    // we make it m+1 because we need to store the start of the next row after the last row
    m_row_index = new int[m + 1];
    
    // Iterate through the arrays and populate the value and column arrays
    for (int i = 0; i < m; i++) {
        // Then we store the starting index of this row in the row index array (so we can better navigate later)
        m_row_index[i] = valuesIndex;

        for (int j = 0; j < n; j++) {
            // checks if the element at the ith row and the jth the column of the matrix (or the corresponding location in the array) is non-zero. 
            // If it is non-zero, we store its value in the m_values array and its column index in the m_col_index array 
            if (i * n + j < arraySize and array[i * n + j] != 0) {
                // As i increases in the for loop this ensures that we can accurately keep track of and go to where we
                // want to go in a linearised 2d array
                m_values[valuesIndex] = array[i * n + j];
                m_col_index[valuesIndex] = j;
                valuesIndex++;
            }
        }
    }
    // Store the starting index of the next row after we're done with the last row in the row array
    m_row_index[m] = valuesIndex;

    // Finally we update the member variables when we're done compressing
    m_nonzeros = non_zero_count;
    m_n = n;
    m_m = m;
    
}
int CSR::getAt(int row, int  col) const{
    // Check if the user passed row and col are within the valid range of the matrix dimensions 
    // (0 bein a valid dimension)
    if (row < 0 or row >= m_m or col < 0 or col >= m_n) {
        throw runtime_error("Exception Error: Row or column index out of range in the CSR matrix");
    }

    // If the request is valid and get past our checks then we can then
    // iterate to the point specified in the arrays then return the value we find
    for (int j = m_row_index[row]; j < m_row_index[row + 1]; j++) {
        if (m_col_index[j] == col) {
            return m_values[j];
        }
    }
    // If the loop finishes without finding a match, it shows that the passed row/col entry is zero in the matrix 
    return 0;
    
} 
bool CSR::operator==(const CSR & rhs) const{
    bool result = true;
    // First compare the number of non-zero values from matrices
        result = result and (m_nonzeros == rhs.m_nonzeros);
        
        // The compare the number of rows
        result = result and (m_m == rhs.m_m);
        
        // Compare the number of columns
        result = result and (m_n == rhs.m_n);

        // Compare the non-zero values
        for (int i = 0; i < m_nonzeros and result; i++) {
            result = result and (m_values[i] == rhs.m_values[i]);
        }

        // Compare the column indices
        for (int j = 0; j < m_nonzeros and result; j++) {
            result = result and (m_col_index[j] == rhs.m_col_index[j]);
        }

        // Compare the row indices
        for (int z = 0; z < m_m and result; z++) {
            result = result and (m_row_index[z] == rhs.m_row_index[z]);
        }
    // Once we've compared every attribute of both matrices we can then return result
    // (if at any point any of these compararisons returns false then result== false and we will know that the objs arent the same)
    return result;
    
}
int CSR::sparseRatio(){
    // First we check for if the matrix is empty
    if (empty()) {
        return 0;
    }
    
    // Then we Check if all members of the matrix are zero
    if (m_nonzeros == 0) {
        return 100;
    }

    // If neither of those are true we can then calculate total number of elements
    int total_elements = m_m * m_n;
    
    // Then calculate the number of zero elements
    int zero_elements = total_elements - m_nonzeros;

    // since we know the ratio is going to be a double we cast it as such
    double ratio = (double)zero_elements / total_elements * 100;

    return ratio;
}
void CSR::dump(){
    cout << endl;
    if (!empty()){
        for (int i=0;i<m_nonzeros;i++)
            cout << m_values[i] << " ";
        cout << endl;
        for (int i=0;i<m_nonzeros;i++)
            cout << m_col_index[i] << " ";
        cout << endl;
        for (int i=0;i<m_m+1;i++)
            cout << m_row_index[i] << " ";
    }
    else
        cout << "The object is empty!";
    cout << endl;
}

//////////////CSRList Class Implementation///////////////
CSRList::CSRList(){
    m_size = 0;
    m_head = nullptr;
    
}
CSRList::CSRList(const CSRList & rhs){
    // zero out attriubutes
    m_head = nullptr;
    m_size = 0;
    // we use this to keep track of the last place in the list
    // so we can avoid inserting from the head which is not a behaviour we want to happen with this constructor
    CSR* m_tail = nullptr;
    CSR* currNode = rhs.m_head;
    // loop through entire list and insert or "copy" everything you see
    while (currNode != nullptr) {
        CSR* copyNode = new CSR(*currNode);
        if (m_head == nullptr) {
            // The first node will be equal to both the head and tail so we can keep track of the last node and insert there
            m_head = copyNode;
            m_tail = copyNode;  
        } else {
            // Connect copyNode to the end of the list
            m_tail->m_next = copyNode;  
            // Update the tail to point to the new last node which is copynode
            m_tail = copyNode;  
        }
        m_size++;
        // move list forward
        currNode = currNode->m_next;
    }
    
}


CSRList::~CSRList(){
    clear();
  
}
bool CSRList::empty() const{
    return m_size == 0;
}
void CSRList::insertAtHead(const CSR & matrix){
    // Dynamically allocate memory to create new node to hold matrix values
    // Use copy constructor to copy all the data
    CSR * copyNode = new CSR(matrix);

    // Update the m_next of the new node to point to the new head
    copyNode->m_next = m_head;
    
    // Point the head to the new node
    m_head = copyNode;

    // Increment the size of the list after all the insertion operations are finished
    m_size++;
    
}
void CSRList::clear(){    
    CSR * currNode = m_head;
    while (currNode != nullptr) {
      // store the next node before deleting the currNoderent node
      CSR * nextNode = currNode ->m_next;
      // delete the currNoderent node
      delete currNode;
      // move on to the next node
      currNode = nextNode;
    }
    // set the head and tail pointers to null and size to 0 to indicate an empty list
    m_head = nullptr;
    m_size = 0;
}

int CSRList::getAt(int CSRIndex, int row, int col) const{
    CSR* currNode = m_head;

    // Iterate to the point in the list that the user specificed 
    for (int i = 0; i < CSRIndex and currNode != nullptr; i++) {
        currNode = currNode->m_next;
    }

    // If the node doesn't exist, we throw a runtime_error
    if (currNode == nullptr) {
        throw runtime_error("Exception Error: Object is Empty");
    }

    // Next we check if the row and col are within the valid range of the matrix dimensions
    if (row < 0 or row >= currNode->m_m or col < 0 or col >= currNode->m_n) {
        throw runtime_error("Exception Error: Row or column index out of range in the CSR matrix");
    }

    // If the request is valid and get past all our checks then we can then
    // loop to the point specified in the arrays then return the value we find
    for (int j = currNode->m_row_index[row]; j < currNode->m_row_index[row + 1]; j++) {
        if (currNode->m_col_index[j] == col) {
            return currNode->m_values[j];
        }
    }
    // If the loop finishes without finding a match, it shows that the row, col entry is zero in the matrix 
    return 0;
    
}
bool CSRList::operator== (const CSRList & rhs) const{
    bool result = 0;
    CSR* rhsNode = rhs.m_head;
    CSR* lhsNode = m_head;
    while (lhsNode!= nullptr and rhsNode!= nullptr)
    {
        // First compare the number of non-zero values from matrices
        result = result and (lhsNode->m_nonzeros == rhsNode->m_nonzeros);
        
        // The compare the number of rows
        result = result and (lhsNode->m_m == rhsNode->m_m);
        
        // Compare the number of columns
        result = result and (lhsNode->m_n == rhsNode->m_n);

        // Compare the non-zero values
        for (int i = 0; i < lhsNode->m_nonzeros; i++) {
            result = result and (lhsNode->m_values[i] == rhsNode->m_values[i]);
        }

        // Compare the column indice
        for (int j = 0; j < lhsNode->m_nonzeros; j++) {
            result = result and (lhsNode->m_col_index[j] == rhsNode->m_col_index[j]);
        }

        // Compare the row indices
        for (int z = 0; z <= lhsNode->m_m; z++) {
            result = result and (lhsNode->m_row_index[z] == rhsNode->m_row_index[z]);
        }

        // Move to the next nodes in both lists
        lhsNode = lhsNode->m_next;
        rhsNode = rhsNode->m_next;
    }

    // finally we check if both lists have reached their ends, ensuring they are of the same length and that they are in fact equal
    result = result and (lhsNode == nullptr and rhsNode == nullptr);

    return result;
    }
const CSRList& CSRList::operator=(const CSRList & rhs){
    // Check for self-assignment.
    if (this != &rhs) { 
        // Iterate and delete all nodes in the og list.
        while (m_head != nullptr) {
            CSR* tempNode = m_head;
            m_head = m_head->m_next;
            delete tempNode;
        }
        m_size = 0;

        // Pointer responsible for telling us what needs to be copied from the og list
        // ie so if it doesnt equal nullptr we keep copying from it 
        CSR* copyListnode = rhs.m_head;
        // Node to keep track of where we currNoderently are when copying from the og list
        CSR* lastCopiednode = nullptr;
        while (copyListnode != nullptr) {
            // allocate memory to make new node
            CSR* copyNode = new CSR();

            // copy the matrix data over
            copyNode->m_nonzeros = copyListnode->m_nonzeros;
            
            copyNode->m_values = new int[copyNode->m_nonzeros];
            int i = 0;
            while (i < copyNode->m_nonzeros) {
                copyNode->m_values[i] = copyListnode->m_values[i];
                i++;
            }

            copyNode->m_col_index = new int[copyNode->m_nonzeros];
            int j = 0;
            while (j < copyNode->m_nonzeros) {
                copyNode->m_col_index[j] = copyListnode->m_col_index[j];
                j++;
            }

            // copying matrix dimensions from source node (copyListnode) to destination node (copyNode)
            copyNode->m_m = copyListnode->m_m;
            copyNode->m_n = copyListnode->m_n;
            // +1 to ensure that the new node's matrix dimensions match the matrix dimensions of the node from where data is being copied from
            copyNode->m_row_index = new int[copyNode->m_m + 1];
            int z = 0;
            while (z <= copyNode->m_m) {
                copyNode->m_row_index[z] = copyListnode->m_row_index[z];
                z++;
            }
            // condition for if this is the first node thats being copied.
            if (m_head == nullptr) { 
                m_head = copyNode;
                lastCopiednode = copyNode;
            } 
            // else if this isnt the first node being copied move the last copied node as such.
            else { 
                lastCopiednode->m_next = copyNode;
                lastCopiednode = copyNode;
            }
            // Move the copy node so we can iterate through the list and keep copying 
            copyListnode = copyListnode->m_next;
            m_size++;
        }
    }
    // Return dereferenced this if the objects turn out to be the same
    return *this;
    
}
int CSRList::averageSparseRatio(){
    // First check If the list is empty,and if it is return 0
    if (empty()) {
        return 0;
    }
    // Initialise values to keep track of the ratios,and number of csr objs in the list
    double sum = 0;  
    int csrCount = 0; 

    CSR* curr = m_head;
    while (curr != nullptr) {
        sum += curr->sparseRatio();
        // increment the csr count for every csr node we come across
        csrCount++;
        // move the list forward
        curr = curr->m_next;
    }
    // calculate average
    double averageSP = (sum / csrCount) * 100;

    // convert average to whole integer by casting it
    int integer_Average_SP = (int)(averageSP + 0.5);

    return integer_Average_SP;
}
void CSRList::dump(){
    if (!empty()){
        CSR* temp = m_head;
        while (temp != nullptr){
            temp->dump();
            temp = temp->m_next;
        }
    }
    else
        cout << "Error: List is empty!" << endl;
}
