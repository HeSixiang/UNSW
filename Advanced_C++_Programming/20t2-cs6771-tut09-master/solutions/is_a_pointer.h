namespace traits {

template <typename T>
struct is_a_pointer {
	static auto constexpr value = false;
};

template <typename T>
struct is_a_pointer<T*> {
	static auto constexpr value = true;
};

} // namespace traits