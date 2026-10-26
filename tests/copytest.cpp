#include "List.h"

int main() {
    // Setup
    PythonList a;
    a.append(10);
    a.append(20);
    a.append(std::string("Hello"));

    std::cout << "Original a: " << a << "\n\n";

    // 1. Copy construction
    std::cout << "--- Copy Construction ---\n";
    PythonList b = a;

    std::cout << "a: " << a << '\n';
    std::cout << "b: " << b << '\n';

    b.append(30);

    std::cout << "After b.append(30):\n";
    std::cout << "a: " << a << '\n';  // Should remain unchanged
    std::cout << "b: " << b << "\n\n";

    // 2. Copy assignment
    std::cout << "--- Copy Assignment ---\n";
    PythonList c;
    c.append(999);
    c = a;

    std::cout << "a: " << a << '\n';
    std::cout << "c: " << c << '\n';

    c.append(40);

    std::cout << "After c.append(40):\n";
    std::cout << "a: " << a << '\n';  // Should remain unchanged
    std::cout << "c: " << c << "\n\n";

    // 3. Move construction
    std::cout << "--- Move Construction ---\n";
    PythonList d = std::move(b);

    std::cout << "d (destination): " << d << '\n';
    std::cout << "b (moved-from): " << b << "\n\n";

    // 4. Move assignment
    std::cout << "--- Move Assignment ---\n";
    PythonList e;
    e.append(777);

    e = std::move(c);

    std::cout << "e (destination): " << e << '\n';
    std::cout << "c (moved-from): " << c << "\n\n";

    // 5. Verify the destinations are still usable
    std::cout << "--- Post-Move Usability ---\n";
    d.append(50);
    e.append(60);

    std::cout << "d after append: " << d << '\n';
    std::cout << "e after append: " << e << '\n';

    return 0;
}
