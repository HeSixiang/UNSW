#include <functional>
#include <iostream>
#include <string>

#include "./animal.h"
#include "./cat.h"
#include "./dog.h"

void do_things(Animal& animal) {
	std::cout << "animal has size " << sizeof(animal) << '\n';
	animal.print_details(" is doing things");
}

int main() {
	// TODO(lecture): Does it make sense to be able to create animals on their own?
	// Fix after explaining the following two sections.
	// auto animal = Animal(5);
	// auto cat = Cat("kitty", 9);
	// auto dog = Dog("doggo", 10);
	// std::cout << '\n';

	// // TODO(lecture): What do we expect the answers here are?
	// std::cout << "animal: size = " << sizeof(animal) << ", ptr = " << sizeof(&animal)
	//           << ", ref = " << sizeof(std::ref(animal)) << '\n';
	// std::cout << "kitty: size = " << sizeof(cat) << ", ptr = " << sizeof(&cat)
	//           << ", ref = " << sizeof(std::ref(cat)) << '\n';
	// std::cout << "doggo: size = " << sizeof(dog) << ", ptr = " << sizeof(&dog)
	//           << ", ref = " << sizeof(std::ref(dog)) << '\n';
	// std::cout << '\n';

	// std::cout << "kitty age = " << cat.get_age() << '\n';
	// // std::cout << "doggo age = " < dog.get_age() << '\n';
	// std::cout << '\n';

	// // TODO(lecture): What do we expect to happen when we uncomment this?
	// do_things(animal);
	// do_things(cat);
	// do_things(dog);
	// std::cout << '\n';

	// LocalFile lf;
	// fn(lf);
	// File f = File::open("/tmp/hello");
}

class File {
public:
	File(std::string_view filename)
	: filename_(filename) {}
	std::string readline() {}
	std::string read() {
		return readline();
	}

	// static LocalFile open_local_file();
	// static NetworkFile open_network_file();

	static File& open(std::string_view filename) {
		if (file_is_local) {
			return std::make_unique<LocalFile>(filename));
		}
		else if (file_is_network) {
			return std::make_unique<NetworkFile>(filename);
		}
		return *files_[filename];
	}

	// static std::flat_hash_map<std::string, std::unique_ptr<File>> files_;

private:
	std::string_view filename_;
};

class LocalFile : private File {
public:
	LocalFile(std::string_view filename)
	: File(filename) {}
	std::string readline() {}
};

// std::unique_ptr<File> lf = std::make_unique<LocalFile>("/tmp/blah");
// lf->read();
// std::unique_ptr<File>& f = File::open("/tmp/blah");

// class Animal {
// 	Animal()
// 	: age_{5}
// 	, blah_{5} {}
// 	int age_;
// 	int blah_;
// } class LandAnimal : public Animal {
// 	...
// } class Dog : public LandAnimal {
// 	Dog(std::string name)
// 	: LandAnimal(name)
// 	, name_(std::move(name)) {
// 		WagTail();
// 	}
// 	std::string name_;
// }

// Dog d;