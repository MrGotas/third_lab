package hashtable_test

import (
	"GO/hashtable"
	"encoding/binary"
	"os"
	"testing"
)

func TestCreateHashTable(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	if hashTable == nil {
		t.Fatal("Failed to create hash table")
	}
	if hashTable.TableSize() != 10 {
		t.Fatalf("Expected table size to be 10, got %d", hashTable.TableSize())
	}
}

func TestPushExistingKey(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	err := hashTable.Push("key1", "value1")
	if err != nil {
		t.Fatalf("Failed to insert key1: %v", err)
	}
	err = hashTable.Push("key1", "value2")
	if err == nil {
		t.Fatal("Expected error when inserting existing key")
	}
	if err.Error() != "key already exists" {
		t.Fatalf("Expected 'key already exists' error, got: %v", err)
	}
}

func TestPushSuccess(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	err := hashTable.Push("key1", "value1")
	if err != nil {
		t.Fatalf("Failed to insert key1: %v", err)
	}

	value, err := hashTable.Get("key1")
	if err != nil {
		t.Fatalf("Failed to get key1: %v", err)
	}
	if value != "value1" {
		t.Fatalf("Expected value 'value1', got %v", value)
	}
}
func TestGetKeyNotFound(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	_, err := hashTable.Get("key1")
	if err == nil {
		t.Fatal("Expected error when key is not found")
	}
	if err.Error() != "key not found" {
		t.Fatalf("Expected 'key not found' error, got: %v", err)
	}
}

func TestGetKeyFound(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	hashTable.Push("key1", "value1")
	value, err := hashTable.Get("key1")
	if err != nil {
		t.Fatalf("Failed to get key1: %v", err)
	}
	if value != "value1" {
		t.Fatalf("Expected value 'value1', got %v", value)
	}
}

func TestDeleteKeySuccess(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	hashTable.Push("key1", "value1")
	err := hashTable.Delete("key1")
	if err != nil {
		t.Fatalf("Failed to delete key1: %v", err)
	}

	_, err = hashTable.Get("key1")
	if err == nil {
		t.Fatal("Expected error when key is deleted")
	}
	if err.Error() != "key not found" {
		t.Fatalf("Expected 'key not found' error, got: %v", err)
	}
}

func TestDeleteKeyNotFound(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	err := hashTable.Delete("nonexistent")
	if err == nil {
		t.Fatal("Expected error when deleting non-existing key")
	}
	if err.Error() != "key not found" {
		t.Fatalf("Expected 'key not found' error, got: %v", err)
	}
}

func TestSaveAndLoadFromFile(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	hashTable.Push("key1", "value1")
	hashTable.Push("key2", "value2")

	err := hashTable.SaveToFile("test.txt")
	if err != nil {
		t.Fatalf("Failed to save to file: %v", err)
	}

	loadedTable := hashtable.NewHashTable(10)
	err = loadedTable.LoadFromFile("test.txt")
	if err != nil {
		t.Fatalf("Failed to load from file: %v", err)
	}

	value, err := loadedTable.Get("key1")
	if err != nil || value != "value1" {
		t.Fatalf("Failed to get key1 after loading from file: %v", err)
	}

	value, err = loadedTable.Get("key2")
	if err != nil || value != "value2" {
		t.Fatalf("Failed to get key2 after loading from file: %v", err)
	}

	// Cleanup
	err = os.Remove("test.txt")
	if err != nil {
		t.Fatalf("Failed to remove test file: %v", err)
	}
}

func TestSaveToBinaryFile(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	hashTable.Push("name", "John Doe")

	err := hashTable.SaveToBinaryFile("test_data.bin")
	if err != nil {
		t.Fatalf("Failed to save to binary file: %v", err)
	}

	// Check if the binary file was created and has content
	file, err := os.Open("test_data.bin")
	if err != nil {
		t.Fatalf("Failed to open binary file: %v", err)
	}
	defer file.Close()

	// Reading and verifying content would depend on your binary structure
	var keyLen, valueLen int32
	var key []byte
	var value []byte

	// Read key and value lengths
	err = binary.Read(file, binary.LittleEndian, &keyLen)
	if err != nil {
		t.Fatalf("Failed to read key length from binary file: %v", err)
	}

	key = make([]byte, keyLen)
	_, err = file.Read(key)
	if err != nil {
		t.Fatalf("Failed to read key from binary file: %v", err)
	}

	err = binary.Read(file, binary.LittleEndian, &valueLen)
	if err != nil {
		t.Fatalf("Failed to read value length from binary file: %v", err)
	}

	value = make([]byte, valueLen)
	_, err = file.Read(value)
	if err != nil {
		t.Fatalf("Failed to read value from binary file: %v", err)
	}

	// Verify content
	if string(key) != "name" || string(value) != "John Doe" {
		t.Fatalf("Binary content is incorrect: key = %s, value = %s", key, value)
	}

	// Cleanup
	err = os.Remove("test_data.bin")
	if err != nil {
		t.Fatalf("Failed to remove binary file: %v", err)
	}
}

func TestLoadFromBinaryFile(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	hashTable.Push("name", "John Doe")

	err := hashTable.SaveToBinaryFile("test_data.bin")
	if err != nil {
		t.Fatalf("Failed to save to binary file: %v", err)
	}

	loadedTable := hashtable.NewHashTable(10)
	err = loadedTable.LoadFromBinaryFile("test_data.bin")
	if err != nil {
		t.Fatalf("Failed to load from binary file: %v", err)
	}

	// Verify that data was loaded correctly
	value, err := loadedTable.Get("name")
	if err != nil || value != "John Doe" {
		t.Fatalf("Failed to get key after loading from binary file: %v", err)
	}

	// Cleanup
	err = os.Remove("test_data.bin")
	if err != nil {
		t.Fatalf("Failed to remove binary file: %v", err)
	}
}

func TestPrint(t *testing.T) {
	hashTable := hashtable.NewHashTable(10)
	err := hashTable.Push("key1", "value1")
	if err != nil {
		t.Fatalf("Failed to insert key1: %v", err)
	}

	err = hashTable.Print()
	if err != nil {
		t.Fatalf("Failed to print hash table: %v", err)
	}
}
