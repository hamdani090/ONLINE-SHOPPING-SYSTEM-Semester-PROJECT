#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cctype>
#include <fstream>

using namespace std;

const int MAX_PRODUCTS = 100;  // Increased to accommodate more products
const int MAX_USERS = 50;
const int MAX_ORDERS = 200;
const int MAX_CART_ITEMS = 20;
const int MAX_ORDER_ITEMS = 20;

// File names for persistence
const string USERS_FILE = "users.txt";
const string PRODUCTS_FILE = "products.txt";
const string ORDERS_FILE = "orders.txt";

// Validation constants
const int MIN_USERNAME_LENGTH = 3;
const int MAX_USERNAME_LENGTH = 15;
const int MIN_PASSWORD_LENGTH = 6;
const int MAX_PASSWORD_LENGTH = 20;
const int ACCOUNT_NUMBER_LENGTH = 16;  // Standard bank account number length

class Product {
protected:
    int id;
    string name;
    string category;
    double pricePKR;

public:
    Product(int i = 0, const string& n = "", const string& cat = "", double p = 0.0)
        : id(i), name(n), category(cat), pricePKR(p) {}

    virtual ~Product() {}

    virtual void displayDetails() const = 0;
    virtual double calculatePrice(int quantity) const {
        return pricePKR * quantity;
    }
    
    virtual string getType() const = 0;
    virtual string getSubCategory() const = 0;

    int getId() const { return id; }
    string getName() const { return name; }
    string getCategory() const { return category; }
    double getBasePrice() const { return pricePKR; }
    
    virtual string toFileString() const {
        stringstream ss;
        ss << getType() << "|" << id << "|" << name << "|" << category << "|" 
           << pricePKR << "|" << getSubCategory();
        return ss.str();
    }
};

class FashionProduct : public Product {
    string subCategory;
public:
    FashionProduct(int i = 0, const string& n = "", double p = 0.0, const string& sub = "")
        : Product(i, n, "Fashion", p), subCategory(sub) {}
    
    string getType() const override { return "FASHION"; }
    string getSubCategory() const override { return subCategory; }
    
    void displayDetails() const override {
        cout << "[ID: " << id << "] " << name << " | Category: " << category
             << " (" << subCategory << ") | Price: PKR " << fixed << setprecision(2) << pricePKR << endl;
    }
};

class EducationProduct : public Product {
    string subCategory;
public:
    EducationProduct(int i = 0, const string& n = "", double p = 0.0, const string& sub = "")
        : Product(i, n, "Education", p), subCategory(sub) {}
    
    string getType() const override { return "EDUCATION"; }
    string getSubCategory() const override { return subCategory; }
    
    void displayDetails() const override {
        cout << "[ID: " << id << "] " << name << " | Category: " << category
             << " (" << subCategory << ") | Price: PKR " << fixed << setprecision(2) << pricePKR << endl;
    }
};

class AutomobileProduct : public Product {
    string subCategory;
public:
    AutomobileProduct(int i = 0, const string& n = "", double p = 0.0, const string& sub = "")
        : Product(i, n, "Automobiles", p), subCategory(sub) {}
    
    string getType() const override { return "AUTOMOBILE"; }
    string getSubCategory() const override { return subCategory; }
    
    void displayDetails() const override {
        cout << "[ID: " << id << "] " << name << " | Category: " << category
             << " (" << subCategory << ") | Price: PKR " << fixed << setprecision(2) << pricePKR << endl;
    }
    
    double calculatePrice(int quantity) const override {
        double base = Product::calculatePrice(quantity);
        return base + base * 0.05;  // 5% tax for automobile products
    }
};

class ElectronicsProduct : public Product {
    string subCategory;
public:
    ElectronicsProduct(int i = 0, const string& n = "", double p = 0.0, const string& sub = "")
        : Product(i, n, "Electronics", p), subCategory(sub) {}
    
    string getType() const override { return "ELECTRONICS"; }
    string getSubCategory() const override { return subCategory; }
    
    void displayDetails() const override {
        cout << "[ID: " << id << "] " << name << " | Category: " << category
             << " (" << subCategory << ") | Price: PKR " << fixed << setprecision(2) << pricePKR << endl;
    }
    
    double calculatePrice(int quantity) const override {
        double base = Product::calculatePrice(quantity);
        if (quantity >= 3) {
            return base * 0.90;  // 10% discount for bulk purchase (3 or more)
        }
        return base;
    }
};

class CartItem {
    Product* product;
    int quantity;
public:
    CartItem() : product(NULL), quantity(0) {}
    CartItem(Product* p, int q) : product(p), quantity(q) {}

    void set(Product* p, int q) { product = p; quantity = q; }
    Product* getProduct() const { return product; }
    int getQuantity() const { return quantity; }
    double getTotalPrice() const {
        if (!product) return 0.0;
        return product->calculatePrice(quantity);
    }
    bool isEmpty() const { return product == NULL || quantity <= 0; }
};

class Order {
    static int nextOrderId;
    int orderId;
    string customerUsername;
    string deliveryAddress;
    CartItem items[MAX_ORDER_ITEMS];
    int itemsCount;
    double totalCost;
    string deliveryType;
    double deliveryCharge;
    string paymentMethod;
    string status;

public:
    Order()
        : orderId(0), customerUsername(""), deliveryAddress(""), itemsCount(0),
          totalCost(0.0), deliveryType("Normal"), deliveryCharge(0.0),
          paymentMethod(""), status("Placed") {}

    void initialize(const string& uname, const string& addr, CartItem cart[],
                    int cartCount, const string& pMethod, const string& dType, double baseCost) {
        orderId = nextOrderId++;
        customerUsername = uname;
        deliveryAddress = addr;
        itemsCount = 0;
        for (int i = 0; i < cartCount && i < MAX_ORDER_ITEMS; ++i) {
            items[i] = cart[i];
            itemsCount++;
        }
        paymentMethod = pMethod;
        deliveryType = dType;
        deliveryCharge = (dType == "Urgent") ? 500.0 : 0.0;
        totalCost = baseCost + deliveryCharge;
        status = "Placed";
    }

    int getId() const { return orderId; }
    string getUsername() const { return customerUsername; }
    string getAddress() const { return deliveryAddress; }
    string getStatus() const { return status; }
    double getTotalCost() const { return totalCost; }
    string getPaymentMethod() const { return paymentMethod; }
    string getDeliveryType() const { return deliveryType; }
    double getDeliveryCharge() const { return deliveryCharge; }
    int getItemsCount() const { return itemsCount; }
    const CartItem& getItem(int index) const { return items[index]; }

    void setStatus(const string& s) { status = s; }
    void setOrderId(int id) { orderId = id; }
    
    // Static method to get next order ID
    static int getNextOrderId() { return nextOrderId; }
    
    // Static method to update next order ID if needed
    static void updateNextOrderId(int id) { 
        if (id >= nextOrderId) {
            nextOrderId = id + 1;
        }
    }

    void displayOrder() const {
        cout << "\n--- Order ID: " << orderId << " ---" << endl;
        cout << "  Customer: " << customerUsername << endl;
        cout << "  Address: " << deliveryAddress << endl;
        cout << "  Delivery Type: " << deliveryType << " (" << (deliveryType == "Urgent" ? "3 days" : "5 days") << ")" << endl;
        cout << "  Payment: " << paymentMethod << endl;
        cout << "  Status: " << status << endl;
        cout << "  Items:" << endl;
        for (int i = 0; i < itemsCount; ++i) {
            Product* p = items[i].getProduct();
            if (p) {
                cout << "    - " << p->getName()
                     << " x " << items[i].getQuantity()
                     << " @ PKR " << fixed << setprecision(2) << items[i].getTotalPrice() << endl;
            }
        }
        cout << "  Delivery Charge: PKR " << fixed << setprecision(2) << deliveryCharge << endl;
        cout << "  FINAL TOTAL: PKR " << fixed << setprecision(2) << totalCost << endl;
    }
    
    string toFileString() const {
        stringstream ss;
        ss << orderId << "|" << customerUsername << "|" << deliveryAddress << "|"
           << itemsCount << "|" << totalCost << "|" << deliveryType << "|"
           << deliveryCharge << "|" << paymentMethod << "|" << status;
        return ss.str();
    }
    
    void fromFileString(const string& fileString) {
        stringstream ss(fileString);
        string token;
        string tokens[9];
        int tokenCount = 0;
        
        while (getline(ss, token, '|') && tokenCount < 9) {
            tokens[tokenCount++] = token;
        }
        
        if (tokenCount >= 9) {
            orderId = stoi(tokens[0]);
            customerUsername = tokens[1];
            deliveryAddress = tokens[2];
            itemsCount = stoi(tokens[3]);
            totalCost = stod(tokens[4]);
            deliveryType = tokens[5];
            deliveryCharge = stod(tokens[6]);
            paymentMethod = tokens[7];
            status = tokens[8];
        }
    }
};

int Order::nextOrderId = 1001;

class NTSHOP;

class User {
protected:
    string username;
    string password;
    string address;
public:
    User(const string& u = "", const string& p = "", const string& a = "")
        : username(u), password(p), address(a) {}
    virtual ~User() {}
    virtual void startSession() = 0;
    virtual string getUserType() const = 0;
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getAddress() const { return address; }
    void setAddress(const string& a) { address = a; }
    
    string toFileString() const {
        return getUserType() + "|" + username + "|" + password + "|" + address;
    }
    
    void fromFileString(const string& fileString) {
        stringstream ss(fileString);
        string type;
        getline(ss, type, '|');  // Skip type
        getline(ss, username, '|');
        getline(ss, password, '|');
        getline(ss, address, '|');
    }
};

class Customer : public User {
    CartItem shoppingCart[MAX_CART_ITEMS];
    int cartCount;
    NTSHOP* shopSystem;

public:
    Customer(const string& u = "", const string& p = "", NTSHOP* shop = NULL)
        : User(u, p, ""), cartCount(0), shopSystem(shop) {}
    
    string getUserType() const override { return "CUSTOMER"; }

    void startSession() override;
    void viewCart() const;
    void checkout();
    void viewOrderHistory() const;
    double calculateCartTotal() const;
    bool addToCart(Product* p, int q);
    void clearCart();
};

class Admin : public User {
    NTSHOP* shopSystem;
public:
    Admin(const string& u = "", const string& p = "", NTSHOP* shop = NULL)
        : User(u, p, ""), shopSystem(shop) {}
    
    string getUserType() const override { return "ADMIN"; }
    
    void startSession() override;
    void viewOrders() const;
    void markOrderDelivered();
    void searchCustomer() const;
};

class NTSHOP {
    Product* allProducts[MAX_PRODUCTS];
    int productCount;
    User* allUsers[MAX_USERS];
    int userCount;
    Order allOrders[MAX_ORDERS];
    int orderCount;

public:
    NTSHOP() : productCount(0), userCount(0), orderCount(0) {
        loadUsers();
        loadProducts();
        loadOrders();
        
        // If no admin exists, create default admin
        if (findUser("admin") == NULL) {
            allUsers[userCount++] = new Admin("admin", "admin123", this);
        }
        
        // If no products exist, create default ones
        if (productCount == 0) {
            addDefaultProducts();
        }
    }

    ~NTSHOP() {
        saveData();  // Save all data before destruction
        for (int i = 0; i < productCount; ++i) delete allProducts[i];
        for (int i = 0; i < userCount; ++i) delete allUsers[i];
    }

    // Helper function to validate username
    static bool isValidUsername(const string& username) {
        if (username.length() < MIN_USERNAME_LENGTH || username.length() > MAX_USERNAME_LENGTH) {
            return false;
        }
        // Check if username contains only alphanumeric characters
        for (char c : username) {
            if (!isalnum(c)) {
                return false;
            }
        }
        return true;
    }
    
    // Helper function to validate password
    static bool isValidPassword(const string& password) {
        if (password.length() < MIN_PASSWORD_LENGTH || password.length() > MAX_PASSWORD_LENGTH) {
            return false;
        }
        // Check if password contains at least one digit and one letter
        bool hasDigit = false;
        bool hasLetter = false;
        for (char c : password) {
            if (isdigit(c)) hasDigit = true;
            if (isalpha(c)) hasLetter = true;
        }
        return hasDigit && hasLetter;
    }
    
    // Helper function to validate account number
    static bool isValidAccountNumber(const string& accNum) {
        if (accNum.length() != ACCOUNT_NUMBER_LENGTH) {
            return false;
        }
        // Check if account number contains only digits
        for (char c : accNum) {
            if (!isdigit(c)) {
                return false;
            }
        }
        return true;
    }

    void addDefaultProducts() {
        // ========== FASHION PRODUCTS  ==========
        addProduct(new FashionProduct(1, "Slim Fit Jeans", 3500.0, "Men's Clothing"));
        addProduct(new FashionProduct(2, "Leather Handbag", 6800.0, "Women's Accessories"));
        addProduct(new FashionProduct(3, "Cotton T-Shirt", 1200.0, "Men's Clothing"));
        addProduct(new FashionProduct(4, "Summer Dress", 4500.0, "Women's Clothing"));
        addProduct(new FashionProduct(5, "Leather Belt", 1500.0, "Accessories"));
        addProduct(new FashionProduct(6, "Sports Shoes", 5500.0, "Footwear"));
        addProduct(new FashionProduct(7, "Woolen Sweater", 3800.0, "Winter Wear"));
        addProduct(new FashionProduct(8, "Formal Suit", 12000.0, "Men's Clothing"));
        addProduct(new FashionProduct(9, "Evening Gown", 8500.0, "Women's Clothing"));
        addProduct(new FashionProduct(10, "Running Shoes", 4200.0, "Footwear"));
        addProduct(new FashionProduct(11, "Leather Jacket", 9500.0, "Outerwear"));
        addProduct(new FashionProduct(12, "Casual Shirt", 1800.0, "Men's Clothing"));
        addProduct(new FashionProduct(13, "Skirt", 2800.0, "Women's Clothing"));
        addProduct(new FashionProduct(14, "Wrist Watch", 6500.0, "Accessories"));
        addProduct(new FashionProduct(15, "Sunglasses", 2200.0, "Accessories"));
        addProduct(new FashionProduct(16, "Backpack", 3200.0, "Bags"));
        addProduct(new FashionProduct(17, "Swimwear", 2500.0, "Beachwear"));
        addProduct(new FashionProduct(18, "Tie Set", 1200.0, "Accessories"));
        addProduct(new FashionProduct(19, "Winter Gloves", 800.0, "Winter Wear"));
        addProduct(new FashionProduct(20, "Formal Shoes", 4800.0, "Footwear"));
        
        // ========== EDUCATION PRODUCTS  ==========
        addProduct(new EducationProduct(21, "Basic Geometry Box", 550.0, "Writing Materials"));
        addProduct(new EducationProduct(22, "Scientific Calculator", 2500.0, "Electronics"));
        addProduct(new EducationProduct(23, "Student Backpack", 3200.0, "Bags"));
        addProduct(new EducationProduct(24, "Notebook Set (5 pcs)", 800.0, "Stationery"));
        addProduct(new EducationProduct(25, "Dictionary", 1800.0, "Books"));
        addProduct(new EducationProduct(26, "Watercolor Set", 1200.0, "Art Supplies"));
        addProduct(new EducationProduct(27, "Laptop Bag", 2800.0, "Bags"));
        addProduct(new EducationProduct(28, "School Uniform", 4500.0, "Clothing"));
        addProduct(new EducationProduct(29, "Encyclopedia Set", 9500.0, "Books"));
        addProduct(new EducationProduct(30, "Drawing Board", 1800.0, "Art Supplies"));
        addProduct(new EducationProduct(31, "Pencil Case", 450.0, "Stationery"));
        addProduct(new EducationProduct(32, "Globe", 3200.0, "Educational Tools"));
        addProduct(new EducationProduct(33, "Whiteboard (Small)", 4200.0, "Office Supplies"));
        addProduct(new EducationProduct(34, "Stapler", 350.0, "Office Supplies"));
        addProduct(new EducationProduct(35, "File Folders (Pack of 10)", 600.0, "Office Supplies"));
        addProduct(new EducationProduct(36, "Desk Lamp", 1800.0, "Study Accessories"));
        addProduct(new EducationProduct(37, "Book Shelf", 8500.0, "Furniture"));
        addProduct(new EducationProduct(38, "Project File", 120.0, "Stationery"));
        addProduct(new EducationProduct(39, "College Bag", 3800.0, "Bags"));
        addProduct(new EducationProduct(40, "Study Table", 12500.0, "Furniture"));
        
        // ========== AUTOMOBILE PRODUCTS  ==========
        addProduct(new AutomobileProduct(41, "Brake Pads (Set of 4)", 12500.0, "Car Spare Parts"));
        addProduct(new AutomobileProduct(42, "Car Battery", 18000.0, "Electrical"));
        addProduct(new AutomobileProduct(43, "Engine Oil (5L)", 8500.0, "Lubricants"));
        addProduct(new AutomobileProduct(44, "Car Cover", 4500.0, "Accessories"));
        addProduct(new AutomobileProduct(45, "Tire (17-inch)", 12000.0, "Wheels"));
        addProduct(new AutomobileProduct(46, "Car Air Freshener", 500.0, "Interior"));
        addProduct(new AutomobileProduct(47, "Wiper Blades (Pair)", 2200.0, "Maintenance"));
        addProduct(new AutomobileProduct(48, "Car Vacuum Cleaner", 3800.0, "Cleaning"));
        addProduct(new AutomobileProduct(49, "Jump Starter", 6500.0, "Tools"));
        addProduct(new AutomobileProduct(50, "Car Seat Covers", 7500.0, "Interior"));
        addProduct(new AutomobileProduct(51, "Steering Wheel Cover", 1200.0, "Interior"));
        addProduct(new AutomobileProduct(52, "Car Wash Kit", 2800.0, "Cleaning"));
        addProduct(new AutomobileProduct(53, "GPS Navigation", 9500.0, "Electronics"));
        addProduct(new AutomobileProduct(54, "Dash Camera", 8500.0, "Electronics"));
        addProduct(new AutomobileProduct(55, "Car Audio System", 15000.0, "Electronics"));
        addProduct(new AutomobileProduct(56, "Wheel Alignment", 4500.0, "Services"));
        addProduct(new AutomobileProduct(57, "Car Polish", 1800.0, "Cleaning"));
        addProduct(new AutomobileProduct(58, "Emergency Tool Kit", 5200.0, "Tools"));
        addProduct(new AutomobileProduct(59, "Car Floor Mats", 3200.0, "Interior"));
        addProduct(new AutomobileProduct(60, "Bike Helmet", 4500.0, "Motorcycle"));
        
        // ========== ELECTRONICS PRODUCTS  ==========
        addProduct(new ElectronicsProduct(61, "43-inch 4K Smart TV", 75000.0, "TV"));
        addProduct(new ElectronicsProduct(62, "Core i5 Laptop", 98000.0, "Laptops"));
        addProduct(new ElectronicsProduct(63, "Wireless Headphones", 8500.0, "Audio"));
        addProduct(new ElectronicsProduct(64, "Smartphone 128GB", 45000.0, "Mobile"));
        addProduct(new ElectronicsProduct(65, "Tablet 10-inch", 32000.0, "Tablets"));
        addProduct(new ElectronicsProduct(66, "Gaming Mouse", 3500.0, "Computer Accessories"));
        addProduct(new ElectronicsProduct(67, "Bluetooth Speaker", 6500.0, "Audio"));
        addProduct(new ElectronicsProduct(68, "Smart Watch", 12000.0, "Wearables"));
        addProduct(new ElectronicsProduct(69, "Digital Camera", 55000.0, "Camera"));
        addProduct(new ElectronicsProduct(70, "Printer", 18000.0, "Office Electronics"));
        addProduct(new ElectronicsProduct(71, "External Hard Drive (1TB)", 8500.0, "Storage"));
        addProduct(new ElectronicsProduct(72, "Wireless Router", 4500.0, "Networking"));
        addProduct(new ElectronicsProduct(73, "Gaming Console", 45000.0, "Gaming"));
        addProduct(new ElectronicsProduct(74, "Earphones", 1800.0, "Audio"));
        addProduct(new ElectronicsProduct(75, "Power Bank 20000mAh", 3500.0, "Mobile Accessories"));
        addProduct(new ElectronicsProduct(76, "Monitor 24-inch", 22000.0, "Computer"));
        addProduct(new ElectronicsProduct(77, "Keyboard Mechanical", 5500.0, "Computer Accessories"));
        addProduct(new ElectronicsProduct(78, "Webcam HD", 3800.0, "Computer Accessories"));
        addProduct(new ElectronicsProduct(79, "Air Purifier", 12500.0, "Home Appliances"));
        addProduct(new ElectronicsProduct(80, "Electric Kettle", 2800.0, "Home Appliances"));
    }

    bool addProduct(Product* p) {
        if (productCount >= MAX_PRODUCTS) return false;
        allProducts[productCount++] = p;
        return true;
    }

    Product* getProductById(int id) const {
        for (int i = 0; i < productCount; ++i)
            if (allProducts[i]->getId() == id) return allProducts[i];
        return NULL;
    }

    void displayAllProductsByCategory(const string& cat) const {
        bool found = false;
        cout << "\n--- Products in " << cat << " ---" << endl;
        for (int i = 0; i < productCount; ++i) {
            if (allProducts[i]->getCategory() == cat) {
                allProducts[i]->displayDetails();
                found = true;
            }
        }
        if (!found) cout << "No products found in this category." << endl;
        cout << "--------------------------------\n" << endl;
    }

    void displayAllProducts() const {
        cout << "\n--- All Products (" << productCount << " products) ---" << endl;
        for (int i = 0; i < productCount; ++i)
            allProducts[i]->displayDetails();
        cout << "--------------------------------\n" << endl;
    }

    void displayCategorySummary() const {
        cout << "\n--- Product Categories Summary ---" << endl;
        int fashionCount = 0, educationCount = 0, automobileCount = 0, electronicsCount = 0;
        
        for (int i = 0; i < productCount; ++i) {
            string cat = allProducts[i]->getCategory();
            if (cat == "Fashion") fashionCount++;
            else if (cat == "Education") educationCount++;
            else if (cat == "Automobiles") automobileCount++;
            else if (cat == "Electronics") electronicsCount++;
        }
        
        cout << "Fashion: " << fashionCount << " products" << endl;
        cout << "Education: " << educationCount << " products" << endl;
        cout << "Automobiles: " << automobileCount << " products" << endl;
        cout << "Electronics: " << electronicsCount << " products" << endl;
        cout << "Total: " << productCount << " products" << endl;
        cout << "--------------------------------\n" << endl;
    }

    bool registerCustomer(const string& u, const string& p) {
        if (!isValidUsername(u)) {
            cout << "Invalid username! Must be " << MIN_USERNAME_LENGTH << "-" << MAX_USERNAME_LENGTH 
                 << " characters (letters and numbers only)." << endl;
            return false;
        }
        
        if (!isValidPassword(p)) {
            cout << "Invalid password! Must be " << MIN_PASSWORD_LENGTH << "-" << MAX_PASSWORD_LENGTH 
                 << " characters with at least one letter and one number." << endl;
            return false;
        }
        
        if (findUser(u) != NULL) {
            cout << "Username already exists! Please choose a different username." << endl;
            return false;
        }
        
        if (userCount >= MAX_USERS) {
            cout << "Maximum user limit reached! Cannot register new user." << endl;
            return false;
        }
        
        allUsers[userCount++] = new Customer(u, p, this);
        return true;
    }

    User* findUser(const string& uname) const {
        for (int i = 0; i < userCount; ++i)
            if (allUsers[i]->getUsername() == uname) return allUsers[i];
        return NULL;
    }

    bool addOrder(const Order& o) {
        if (orderCount >= MAX_ORDERS) return false;
        allOrders[orderCount++] = o;
        cout << "\n\n********************************************************" << endl;
        cout << "    Order Placed Successfully! Order ID: " << allOrders[orderCount-1].getId() << endl;
        cout << "********************************************************\n" << endl;
        return true;
    }

    int getOrderCount() const { return orderCount; }
    const Order& getOrderAt(int idx) const { return allOrders[idx]; }
    Order& getOrderAt(int idx) { return allOrders[idx]; }

    void displayAllOrders() const {
        if (orderCount == 0) { cout << "\nNo orders placed yet." << endl; return; }
        for (int i = 0; i < orderCount; ++i) allOrders[i].displayOrder();
    }

    void displayDeliveredOrders() const {
        cout << "\n--- Delivered Orders ---" << endl;
        bool found = false;
        for (int i = 0; i < orderCount; ++i) {
            if (allOrders[i].getStatus() == "Delivered") {
                allOrders[i].displayOrder();
                found = true;
            }
        }
        if (!found) cout << "No delivered orders found." << endl;
    }

    User** getUsersArray() { return allUsers; }
    int getUserCount() const { return userCount; }
    Product** getProductArray() { return allProducts; }
    int getProductCount() const { return productCount; }
    
    void saveData() {
        saveUsers();
        saveProducts();
        saveOrders();
    }
    
    void saveUsers() {
        ofstream outFile(USERS_FILE);
        if (!outFile) {
            cout << "Error: Could not save users to file!" << endl;
            return;
        }
        for (int i = 0; i < userCount; ++i) {
            outFile << allUsers[i]->toFileString() << endl;
        }
        outFile.close();
    }
    
    void saveProducts() {
        ofstream outFile(PRODUCTS_FILE);
        if (!outFile) {
            cout << "Error: Could not save products to file!" << endl;
            return;
        }
        for (int i = 0; i < productCount; ++i) {
            outFile << allProducts[i]->toFileString() << endl;
        }
        outFile.close();
    }
    
    void saveOrders() {
        ofstream outFile(ORDERS_FILE);
        if (!outFile) {
            cout << "Error: Could not save orders to file!" << endl;
            return;
        }
        for (int i = 0; i < orderCount; ++i) {
            outFile << allOrders[i].toFileString() << endl;
        }
        outFile.close();
    }
    
    void loadUsers() {
        ifstream inFile(USERS_FILE);
        if (!inFile) {
            cout << "No existing users file found. Starting fresh." << endl;
            return;
        }
        
        char line[256];  // Fixed size array for line reading
        while (inFile.getline(line, 256) && userCount < MAX_USERS) {
            string strLine(line);
            if (strLine.empty()) continue;
            
            stringstream ss(strLine);
            string type;
            getline(ss, type, '|');
            
            if (type == "ADMIN") {
                Admin* admin = new Admin("", "", this);
                admin->fromFileString(strLine);
                allUsers[userCount++] = admin;
            } else if (type == "CUSTOMER") {
                Customer* customer = new Customer("", "", this);
                customer->fromFileString(strLine);
                allUsers[userCount++] = customer;
            }
        }
        inFile.close();
    }
    
    void loadProducts() {
        ifstream inFile(PRODUCTS_FILE);
        if (!inFile) {
            cout << "No existing products file found. Starting fresh." << endl;
            return;
        }
        
        char line[256];
        while (inFile.getline(line, 256) && productCount < MAX_PRODUCTS) {
            string strLine(line);
            if (strLine.empty()) continue;
            
            stringstream ss(strLine);
            string tokens[6];
            int tokenCount = 0;
            string token;
            
            while (getline(ss, token, '|') && tokenCount < 6) {
                tokens[tokenCount++] = token;
            }
            
            if (tokenCount < 6) continue;
            
            int id = stoi(tokens[1]);
            double price = stod(tokens[4]);
            string name = tokens[2];
            string subCategory = tokens[5];
            
            if (tokens[0] == "FASHION") {
                allProducts[productCount++] = new FashionProduct(id, name, price, subCategory);
            } else if (tokens[0] == "EDUCATION") {
                allProducts[productCount++] = new EducationProduct(id, name, price, subCategory);
            } else if (tokens[0] == "AUTOMOBILE") {
                allProducts[productCount++] = new AutomobileProduct(id, name, price, subCategory);
            } else if (tokens[0] == "ELECTRONICS") {
                allProducts[productCount++] = new ElectronicsProduct(id, name, price, subCategory);
            }
        }
        inFile.close();
    }
    
    void loadOrders() {
        ifstream inFile(ORDERS_FILE);
        if (!inFile) {
            cout << "No existing orders file found. Starting fresh." << endl;
            return;
        }
        
        char line[256];
        while (inFile.getline(line, 256) && orderCount < MAX_ORDERS) {
            string strLine(line);
            if (strLine.empty()) continue;
            
            Order order;
            order.fromFileString(strLine);
            
            // Check if order ID already exists
            bool exists = false;
            for (int i = 0; i < orderCount; i++) {
                if (allOrders[i].getId() == order.getId()) {
                    exists = true;
                    break;
                }
            }
            
            if (!exists) {
                allOrders[orderCount++] = order;
                
                // Update nextOrderId to avoid duplicates using the static method
                Order::updateNextOrderId(order.getId());
            }
        }
        inFile.close();
    }
};

bool Customer::addToCart(Product* p, int q) {
    if (!p || q <= 0) return false;
    if (cartCount >= MAX_CART_ITEMS) return false;
    shoppingCart[cartCount++].set(p, q);
    return true;
}

void Customer::viewCart() const {
    if (cartCount == 0) {
        cout << "\n Your cart is empty." << endl;
        return;
    }
    cout << "\n--- Your Shopping Cart ---" << endl;
    double total = 0.0;
    for (int i = 0; i < cartCount; ++i) {
        const CartItem& item = shoppingCart[i];
        cout << (i + 1) << ". " << item.getProduct()->getName()
             << " x " << item.getQuantity()
             << " | Price: PKR " << fixed << setprecision(2) << item.getTotalPrice() << endl;
        total += item.getTotalPrice();
    }
    cout << "--------------------------------" << endl;
    cout << "Subtotal: PKR " << fixed << setprecision(2) << total << endl;
    cout << "--------------------------------\n" << endl;
}

double Customer::calculateCartTotal() const {
    double total = 0.0;
    for (int i = 0; i < cartCount; ++i) total += shoppingCart[i].getTotalPrice();
    return total;
}

void Customer::clearCart() {
    for (int i = 0; i < cartCount; ++i) shoppingCart[i] = CartItem();
    cartCount = 0;
}

void Customer::checkout() {
    if (cartCount == 0) {
        cout << "\n Cannot checkout. Your cart is empty." << endl;
        return;
    }

    string tempUsername, tempAddress;
    cout << "\n--- Checkout Process ---" << endl;
    cout << "Enter your delivery username: ";
    cin >> tempUsername;
    
    if (tempUsername != this->username) {
        cout << "Username mismatch! Please enter your registered username." << endl;
        return;
    }
    
    cout << "Enter your full delivery address: ";
    cin.ignore();
    getline(cin, tempAddress);
    this->address = tempAddress;

    int paymentChoice;
    string paymentMethod;
    cout << "\nSelect Payment Method (1 for Advance, 2 for Cash on Delivery): ";
    cin >> paymentChoice;

    if (paymentChoice == 1) {
        paymentMethod = "Advance Payment";
        string accountName, accountNumber;
        cout << "Enter Account Name: ";
        cin.ignore();
        getline(cin, accountName);
        
        // Validate account number
        bool validAccount = false;
        while (!validAccount) {
            cout << "Enter Account Number (16 digits): ";
            getline(cin, accountNumber);
            
            if (shopSystem->isValidAccountNumber(accountNumber)) {
                validAccount = true;
            } else {
                cout << "Invalid account number! Must be exactly " << ACCOUNT_NUMBER_LENGTH << " digits." << endl;
                cout << "Please enter again: ";
            }
        }
        cout << "Payment details secured for advance payment." << endl;
    } else {
        paymentMethod = "Cash on Delivery (COD)";
    }

    int deliveryChoice;
    string deliveryType;
    double baseTotal = calculateCartTotal();
    cout << "\nSelect Delivery Type:" << endl;
    cout << "1. Normal Delivery (5 days, No extra charge)" << endl;
    cout << "2. Urgent Delivery (3 days, PKR 500 extra charge)" << endl;
    cout << "Your choice: ";
    cin >> deliveryChoice;

    if (deliveryChoice == 2) {
        deliveryType = "Urgent";
        cout << "Urgent Delivery selected (PKR 500 added to total)." << endl;
    } else {
        deliveryType = "Normal";
    }

    cout << "\n\n--- Order Summary ---" << endl;
    viewCart();
    cout << "Payment: " << paymentMethod << endl;
    cout << "Delivery: " << deliveryType << endl;

    char confirm;
    cout << "Proceed with placing order? (y/n): ";
    cin >> confirm;

    if (tolower(confirm) != 'y') {
        cout << "\nOrder cancelled by user. Cart retained." << endl;
        return;
    }

    Order newOrder;
    newOrder.initialize(this->username, this->address, shoppingCart, cartCount, paymentMethod, deliveryType, baseTotal);

    if (shopSystem->addOrder(newOrder)) {
        clearCart();
        shopSystem->saveData();  // Save data after successful order
    } else {
        cout << "Failed to add order to system." << endl;
    }
}

void Customer::viewOrderHistory() const {
    cout << "\n--- Your Order History ---" << endl;
    bool found = false;
    int oc = shopSystem->getOrderCount();
    for (int i = 0; i < oc; ++i) {
        const Order& o = shopSystem->getOrderAt(i);
        if (o.getUsername() == this->username) {
            o.displayOrder();
            found = true;
        }
    }
    if (!found) cout << "You have no orders yet." << endl;
}

void Customer::startSession() {
    int choice;
    while (true) {
        cout << "\n--- Welcome, " << username << " to N&T SHOP ---" << endl;
        cout << "1. Browse Products by Category" << endl;
        cout << "2. View All Products" << endl;
        cout << "3. View Category Summary" << endl;
        cout << "4. View Cart and Checkout" << endl;
        cout << "5. View Order History " << endl;
        cout << "6. Logout" << endl;
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Invalid input. Please try again." << endl;
            continue;
        }
        if (choice == 6) break;

        if (choice == 1) {
            int catChoice;
            string catName;
            cout << "\n--- Select Category ---" << endl;
            cout << "1: Fashion\n2: Education\n3: Automobiles\n4: Electronics\nYour choice: ";
            cin >> catChoice;
            switch (catChoice) {
                case 1: catName = "Fashion"; break;
                case 2: catName = "Education"; break;
                case 3: catName = "Automobiles"; break;
                case 4: catName = "Electronics"; break;
                default: cout << "Invalid category." << endl; continue;
            }
            shopSystem->displayAllProductsByCategory(catName);

            int productId, quantity;
            cout << "Enter Product ID to add to cart (0 to skip): ";
            if (!(cin >> productId)) {
                cin.clear(); cin.ignore(10000, '\n'); continue;
            }
            if (productId == 0) continue;

            Product* selectedProduct = shopSystem->getProductById(productId);
            if (selectedProduct) {
                cout << "Enter quantity: ";
                if (!(cin >> quantity) || quantity <= 0) {
                    cin.clear(); cin.ignore(10000, '\n');
                    cout << "Invalid quantity. Skipped." << endl;
                    continue;
                }
                if (addToCart(selectedProduct, quantity)) {
                    cout << " Added " << quantity << " x " << selectedProduct->getName() << " to cart." << endl;
                } else {
                    cout << "Failed to add item to cart (cart full)." << endl;
                }
            } else {
                cout << " Invalid Product ID." << endl;
            }
        } else if (choice == 2) {
            shopSystem->displayAllProducts();
            
            int productId, quantity;
            cout << "Enter Product ID to add to cart (0 to skip): ";
            if (!(cin >> productId)) {
                cin.clear(); cin.ignore(10000, '\n'); continue;
            }
            if (productId == 0) continue;

            Product* selectedProduct = shopSystem->getProductById(productId);
            if (selectedProduct) {
                cout << "Enter quantity: ";
                if (!(cin >> quantity) || quantity <= 0) {
                    cin.clear(); cin.ignore(10000, '\n');
                    cout << "Invalid quantity. Skipped." << endl;
                    continue;
                }
                if (addToCart(selectedProduct, quantity)) {
                    cout << " Added " << quantity << " x " << selectedProduct->getName() << " to cart." << endl;
                } else {
                    cout << "Failed to add item to cart (cart full)." << endl;
                }
            } else {
                cout << " Invalid Product ID." << endl;
            }
        } else if (choice == 3) {
            shopSystem->displayCategorySummary();
        } else if (choice == 4) {
            viewCart();
            if (cartCount > 0) {
                char confirm;
                cout << "Ready to checkout? (y/n): ";
                cin >> confirm;
                if (tolower(confirm) == 'y') checkout();
            }
        } else if (choice == 5) {
            viewOrderHistory();
        } else {
            cout << "Invalid option." << endl;
        }
    }
}

void Admin::viewOrders() const {
    shopSystem->displayAllOrders();
}

void Admin::markOrderDelivered() {
    int id;
    cout << "Enter Order ID to mark as delivered: ";
    if (!(cin >> id)) {
        cin.clear(); cin.ignore(10000, '\n');
        cout << "Invalid ID." << endl;
        return;
    }
    int oc = shopSystem->getOrderCount();
    for (int i = 0; i < oc; ++i) {
        Order& o = shopSystem->getOrderAt(i);
        if (o.getId() == id) {
            if (o.getStatus() == "Placed") {
                o.setStatus("Delivered");
                cout << " Order ID " << id << " marked as 'Delivered'." << endl;
                shopSystem->saveData();  // Save after status change
                return;
            } else {
                cout << " Order ID " << id << " is already " << o.getStatus() << "." << endl;
                return;
            }
        }
    }
    cout << " Order ID " << id << " not found." << endl;
}

void Admin::searchCustomer() const {
    string searchKey;
    int searchType;

    cout << "\n--- Search Customer ---" << endl;
    cout << "Search by: 1) Username or 2) Address: ";
    if (!(cin >> searchType)) {
        cin.clear(); cin.ignore(10000, '\n'); return;
    }

    if (searchType == 1) {
        cout << "Enter Username: ";
        cin >> searchKey;
    } else if (searchType == 2) {
        cout << "Enter Address : ";
        cin.ignore();
        getline(cin, searchKey);
    } else {
        cout << "Invalid search type." << endl;
        return;
    }

    cout << "\n--- Search Results ---" << endl;
    bool found = false;

    User** users = shopSystem->getUsersArray();
    int uc = shopSystem->getUserCount();

    for (int i = 0; i < uc; ++i) {
        Customer* customer = dynamic_cast<Customer*>(users[i]);
        if (customer) {
            bool matches = false;
            if (searchType == 1 && customer->getUsername() == searchKey) matches = true;
            if (searchType == 2 && customer->getAddress().find(searchKey) != string::npos && !searchKey.empty()) matches = true;

            if (matches) {
                found = true;
                cout << "Found Customer: " << customer->getUsername() << endl;
                cout << "  - Last Known Address: " << customer->getAddress() << endl;

                double totalSpent = 0.0;
                int ordersCount = 0;
                int oc = shopSystem->getOrderCount();
                for (int j = 0; j < oc; ++j) {
                    const Order& o = shopSystem->getOrderAt(j);
                    if (o.getUsername() == customer->getUsername() && o.getStatus() != "Cancelled") {
                        totalSpent += o.getTotalCost();
                        ordersCount++;
                    }
                }

                cout << "  - Total Orders Placed : " << ordersCount << endl;
                cout << "  - Total Amount Shopped: PKR " << fixed << setprecision(2) << totalSpent << endl;
            }
        }
    }

    if (!found) {
        cout << "No customers found matching the criteria." << endl;
    }
}

void Admin::startSession() {
    int choice;
    while (true) {
        cout << "\n--- Welcome, Admin (" << username << ") ---" << endl;
        cout << "1. View All Orders" << endl;
        cout << "2. View Delivered Orders" << endl;
        cout << "3. Mark Order as Delivered" << endl;
        cout << "4. Search Customer Information" << endl;
        cout << "5. View Product Inventory" << endl;
        cout << "6. View Category Summary" << endl;
        cout << "7. Save All Data" << endl;
        cout << "8. Logout" << endl;
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Invalid input. Please try again." << endl;
            continue;
        }
        if (choice == 8) break;

        switch (choice) {
            case 1: viewOrders(); break;
            case 2: shopSystem->displayDeliveredOrders(); break;
            case 3: markOrderDelivered(); break;
            case 4: searchCustomer(); break;
            case 5: shopSystem->displayAllProducts(); break;
            case 6: shopSystem->displayCategorySummary(); break;
            case 7: shopSystem->saveData(); cout << "All data saved successfully!" << endl; break;
            default: cout << "Invalid option." << endl;
        }
    }
}

void runSystem(NTSHOP* shop) {
    string username, password;
    int roleChoice;

    cout << "\n=======================================" << endl;
    cout << "  WELCOME TO N&T SHOP ONLINE SYSTEM" << endl;
    cout << "=======================================" << endl;

    while (true) {
        cout << "Are you a Customer or an Admin?" << endl;
        cout << "1. Customer\n2. Admin\n3. Register New Customer\n4. Exit" << endl;
        cout << "Enter choice: ";
        if (!(cin >> roleChoice)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Invalid input. Please try again." << endl;
            continue;
        }
        if (roleChoice == 4) break;

        if (roleChoice == 3) {
            string username, password;
            bool validRegistration = false;
            
            while (!validRegistration) {
                cout << "\n--- New Customer Registration ---" << endl;
                cout << "Username requirements: " << MIN_USERNAME_LENGTH << "-" << MAX_USERNAME_LENGTH 
                     << " characters (letters and numbers only)" << endl;
                cout << "Password requirements: " << MIN_PASSWORD_LENGTH << "-" << MAX_PASSWORD_LENGTH 
                     << " characters with at least one letter and one number" << endl;
                
                cout << "Enter new username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                
                if (shop->registerCustomer(username, password)) {
                    cout << "\n Customer '" << username << "' registered successfully!" << endl;
                    shop->saveData();  // Save after registration
                    validRegistration = true;
                } else {
                    cout << "\n Registration failed. Please try again." << endl;
                    cout << "Press 'r' to retry or any other key to cancel: ";
                    char retry;
                    cin >> retry;
                    if (tolower(retry) != 'r') {
                        break;
                    }
                }
            }
            continue;
        }

        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        User* currentUser = shop->findUser(username);

        if (currentUser) {
            bool authenticated = false;
            Admin* admin = dynamic_cast<Admin*>(currentUser);
            Customer* cust = dynamic_cast<Customer*>(currentUser);

            if (admin) {
                if (username == "admin" && password == "admin123") authenticated = true;
            } else if (cust) {
                // For customers, just check if user exists (simplified authentication)
                authenticated = true;
            }

            if (authenticated) {
                if (roleChoice == 1 && cust) {
                    cust->startSession();
                } else if (roleChoice == 2 && admin) {
                    admin->startSession();
                } else {
                    cout << " Role mismatch. Please log in with the correct role." << endl;
                }
            } else {
                cout << " Authentication failed. Incorrect password." << endl;
            }
        } else {
            cout << " User not found. Please try again or register." << endl;
        }
    }
    cout << "\nThank you for using N&T SHOP. Goodbye!" << endl;
}

int main() {
    cout << fixed << setprecision(2);
    NTSHOP* shop = new NTSHOP();
    runSystem(shop);
    delete shop;
    sysrtem("pause");
    return 0;
}