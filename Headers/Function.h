#ifndef __FUNCTION_H__
#define __FUNCTION_H__

//Á¦°ö
template<typename T>
inline T SQUERE(T _val) {
	return _val * _val;
}
//#define SQUERE(val) ((val) * (val))

template <typename T>
constexpr void SafeDelete(T& _ptr) {
	if (_ptr) {
		delete _ptr;
		_ptr = nullptr;
	}
}

template <typename T>
void SafeDelete_Array(T& _ptr) {
	if (_ptr) {
		delete[] _ptr;
		_ptr = nullptr;
	}
}

#endif // !__FUNCTION_H__
