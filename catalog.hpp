
// catalog.hpp
// CSUF CPSC 131, Fall 2016, Project 1
// Brandon Hawkinson
// Classes that define a catalog of supermarket products.

#pragma once

#include <stdexcept>
#include <string>

using namespace std;
// A product represents one particular kind of product, such as apples
// or cereal.
class Product {
public:
  // Create a product with a given code, name, and price.
  //
  // code is intended to be a UPC code (bar code) or PLU code in a
  // string.
  //
  // name may be any string.
  //
  // price is in units of dollars. It must be positive, or else this
  // function throws std::invalid_argument.
  Product(const std::string& newCode,
          const std::string& newName,
          double newPrice) {
    // TODO: implement this function properly
    if (newPrice <= 0)
      throw invalid_argument("Price Must be Positive");
    name = newName;
    code = newCode;
    price = newPrice;
  }

  ~Product() { }

  // Accessors.
  const std::string& getCode() const { return code; }
  const std::string& getName() const { return name; }
  double getPrice() const { return price; }
  
private:
  std::string code, name;
  double price;
  Product *nextProduct; //allows product to have the pointer to the next product to make it work as a node
  friend class Catalog; //allows catalog to access it's node.
};

// A catalog represents a collection of all of the products available
// at a store.
//Catalog is going to be a SLL data structure and the Products the Nodes
class Catalog {
public:
  // Create a new catalog.
  //
  // maxProducts is the maximum number of products that can be
  // stored. It must be positive, or else this function throws
  // std::invalid_argument.
  Catalog(int newMaxProducts) { //set up an empty SLL
    // TODO: implement this function properly
    if (newMaxProducts <= 0)
      throw invalid_argument("Max Products needs to be postive");
    maxProducts = newMaxProducts;
    head = NULL;
  }
  
  ~Catalog() { //delete all the nodes on destruction
    while (!isEmpty()) {
      removeProduct();
    }
  }

  // Accessors.
  int getMaxProducts() const {
    return maxProducts;
  }
  
  int getNumProducts() const {
    return numProducts;
  }

  // Return true when the catalog cannot fit any more products.
  bool isFull() const {
    return numProducts==maxProducts;
  }

  bool isEmpty() const {
    return head == NULL;
  }

  // Add a new product to the catalog with a given code and name.
  //
  // code, name, and price have the same meaning as in a Product. If
  // price is invalid, throw std::invalid_argument.
  //
  // If this catalog is already full, throw overflow_error.
  //
  // Code must be different from all the product codes already in the
  // database. If it's not, throw std::invalid_argument.
  void addProduct(const std::string& code,
                  const std::string& name,
                  double price) {
      if (isFull()) {
          throw overflow_error("Catalog Already Full");
      }
      if (findMatchingCode(&code)) { 
          throw invalid_argument("Product Already in system");
      }
      
    Product* v = new Product(code, name, price);		// create new node
  	v->nextProduct = head;				// head now follows v
  	head = v;				// v is now the head
    numProducts++;
  }
/*I created a new find code function to because the find code function returns a node where find matching Code returns a boolean argument. 
While the idea's and SOME code are the same, ultimately we're after a different item. FindCode is catered towards returning the active node
which is a inheritly different idea to accessing an active node. Therefore the code is not reusable in an effecient way. 
	*/
    bool findMatchingCode(const string* code) const {
        Product* currentProduct = head;
        if (head == NULL) {
            return false;
        }
        bool codeFound = false;
        int counter = 0;
        while (codeFound == false) {
            counter++;
            if (counter > numProducts) {
                return false;
            }
        
            if (*code == *(&currentProduct->code)) {
                return true;
            }
            currentProduct = currentProduct->nextProduct;
        }
        
        return codeFound;

    }
    // Find a product by its code.
    //
    // code is a product code.
    //
    // Returns a const reference to the product with the matching code.
    //
    // Throw std::invalid_argument if no product with that code exists
    // in the catalog.

  const Product& findCode(const std::string& code) const {
    // TODO: implement this function properly
    Product* currentProduct = head;
    
    bool codeFound = false;
    int counter = 0;
    while (codeFound == false) {
      counter++;
      if (counter > numProducts) { //stops it at the NULL pointer, throws the argument. 
        throw invalid_argument ("Product not found");
      }
      if (code == currentProduct->getCode()) {
        codeFound = true;
        break; //breaks the current while loop to return the active product 
      }
      currentProduct = currentProduct->nextProduct; //iterate to the next product
    }

    return *(currentProduct); //return the product in which the code was found. 
  }

  void removeProduct() { //
    //from SLL code from instructors to delete a node(product in this case).
    if (isEmpty()) throw length_error("empty list");
	  Product* old = head;			// save current head
	  head = old->nextProduct;			// skip over old head
	  delete old;				// delete the old head
    numProducts--;
  }

private:
  Product* head;//allows the catalog to act like the SLL data structure 
  int maxProducts, numProducts;
};
