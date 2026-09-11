#include <iostream>
#include <filesystem>
#include <stdfloat>
#include <numeric>
#include <vector>
#include <cstdint>
#include <memory>
#include <algorithm>
#include <random>
#include <string>
#include <deque>

//i lose by human engineering, but i win by human engineering not

namespace fs = std::filesystem;

using F16 = float;

template<typename T>
class Rational {
public:	
	Rational(T a, T b) : A(a), B(b) {}
	Rational(T N) : A(N), B(1) {}
	Rational(float N) : A(N), B(1) {}

	bool Denomi() {
		T AA = std::gcd(A, B);
		A /= AA;
		B /= AA;
		return true;
	}

	Rational<T> operator+(const Rational<T>& other) const {
		T newA = A * other.B + other.A * B;
		T newB = B * other.B;
		auto R = Rational<T>(newA, newB);
		R.Denomi();
		return R;
	}


	Rational<T> operator-(const Rational<T>& other) const {
		T newA = A * other.B - other.A * B;
		T newB = B * other.B;
		auto R = Rational<T>(newA, newB);
		R.Denomi();
		return R;
	}
	Rational<T> operator*(const Rational<T>& other) const {
		T newA = A * other.A;
		T newB = B * other.B;
		auto R = Rational<T>(newA, newB);
		R.Denomi();
		return R;
	}

	Rational<T> operator/(const Rational<T>& other) const {
		T newA = A * other.B;
		T newB = B * other.A;
		auto R = Rational<T>(newA, newB);
		R.Denomi();
		return R;
	}

	T Devee() {
		return A;
	}

	T Dever(){
		return B;
	}

	operator std::intmax_t() const {
		Denomi();
		return static_cast<std::intmax_t>(A) / static_cast<std::intmax_t>(B);
	}
	operator float() const {
		Denomi();
		return static_cast<float>(A) / static_cast<float>(B);
	}


	float Divaide() {
		//Denomi();
		return static_cast<float>(A) / static_cast<float>(B);
	}


protected:
	T A = 0;
	T B = 1;
};

template<class T>
struct Surface {
	T width;
	T height;
	std::vector<T> data;

	Surface(T w, T h) : width(w), height(h) {
		data.resize(width * height);
	}
	T area() const {
		return width * height;
	}

	bool SetPixel(T x, T y, T value) {
		if (x < 0 || x >= width || y < 0 || y >= height) {
			return false;
		}
		data[y * width + x] = value;
		return true;
	}
	T& Index(T x, T y) {
		return data[y * width + x];
	}
};

template<class T>
struct Palse {
	Palse(std::intmax_t size) : data(size) {}

	std::vector<T> data;
};

struct EntarTainmentLayer {
	Surface<Rational<std::intmax_t>> surface,ZBuffer;
	Palse<Rational<std::intmax_t>> palse;
	EntarTainmentLayer(F16 width, F16 height, std::intmax_t palse_size) : surface(static_cast<std::intmax_t>(width), static_cast<std::intmax_t>(height)), palse(palse_size) {
		palse.data.resize(static_cast<std::size_t>(width * height));
	}
};

struct Corutine {
	struct ISuspendable {
		virtual bool Suspend() = 0;
		virtual bool IsSuspended() = 0;
	};

	struct IWakeUp {
		virtual bool WakeUp() = 0;
	};

	struct IUpdateable : ISuspendable, IWakeUp {
		virtual bool Update(Corutine& corutine) = 0;
	};

	bool Update(Corutine& corutine) {
		for (auto& updateable : updateables) {
			if (!updateable->Update(corutine)) {
				return false;
			}
		}
		return true;
	}
	bool Swtich(IUpdateable& other) {
		other.Update(*this);
		other.Suspend();
		auto A = updateables;
		std::shuffle(updateables.begin(), updateables.end(), std::mt19937(std::random_device()()));
		this->Update(*this);
		updateables = A;
		other.WakeUp();

		return true;
	}

	bool AddUpdateable(std::shared_ptr<IUpdateable> updateable) {
		updateables.push_back(updateable);
		return true;
	}

protected:
	std::vector<std::shared_ptr<IUpdateable>> updateables;
};

struct UTF8_One {

	struct {
		union {
			union Conv {
				std::uint32_t ICHAR = 0;
				std::uint8_t CChar[4];
			};
			struct Buf {
				std::int32_t __ = 0;
			};
		};
	};


	std::uint8_t Char[4] = { 0,0,0,'\0'};
	std::uint8_t Length = 0;

	operator char* () {
		return reinterpret_cast<char*>(Char);
	}
};

UTF8_One ParseOne(const char* P, int I, int L) {
	UTF8_One U;
	if (I < L) {
		U.Char[0] = static_cast<std::uint8_t>(P[I]);
		if ((U.Char[0] & 0x80) == 0) {
			return U;
		}
		else if ((U.Char[0] & 0xE0) == 0xC0) {
			if (I + 1 < L) {
				U.Char[1] = P[I + 1];
				U.Length = 1;
				return U;
			}
		}
		else if ((U.Char[0] & 0xF0) == 0xE0) {
			if (I + 2 < L) {
				U.Char[1] = P[I + 1];
				U.Char[2] = P[I + 2];
				U.Length = 2;
				return U;
			}
		}
		else if ((U.Char[0] & 0xF8) == 0xF0) {
			if (I + 3 < L) {
				U.Char[1] = P[I + 1];
				U.Char[2] = P[I + 2];
				U.Char[3] = P[I + 3];
				U.Length = 3;
				return U;
			}
		}
	}
	return U;
}

struct Screen {
	std::intmax_t width;
	std::intmax_t height;
	std::vector<UTF8_One> buffer;

	UTF8_One& Index(std::intmax_t x, std::intmax_t y) {
		return buffer[y * width + x];
	}
};

class DosWindow : public Corutine::IUpdateable {
public:
	DosWindow() : layer(80, 25, 2000), screen{ 80, 25 }, Back{ 80, 25 } {
		screen.buffer.resize(screen.width * screen.height);
		Back.buffer.resize(Back.width * Back.height);
	}

	void SetCharcter(std::intmax_t x, std::intmax_t y, UTF8_One character) {
		if (x < 0 || x >= screen.width || y < 0 || y >= screen.height) {
			return;
		}
		screen.Index(x, y) = character;
	}

	void WriteLine(std::intmax_t y, std::vector<UTF8_One> line) {
		if (y < 0 || y >= screen.height) {
			return;
		}
		for (std::intmax_t x = 0; x < line.size() && x < screen.width; ++x) {
			screen.Index(x, y) = line[x];
		}
		BLines.push_front(line);
	}

	Corutine corutine;

	bool Suspend() override { return true; }
	bool IsSuspended() override { return false; }
	bool WakeUp() override { return true; }

	bool Update(Corutine& corutine) override {
		// Update logic for the DosWindow
		// For example, you can render the screen buffer to the console here
		for (std::intmax_t y = 0; y < screen.height; ++y) {
			for (std::intmax_t x = 0; x < screen.width; ++x) {
				std::cout << screen.Index(x, y);
			}
			std::cout << std::endl;
		}
		return true;
	}

protected:
	EntarTainmentLayer layer;
	Screen screen, Back;
	std::deque<std::vector<UTF8_One>> BLines;
};

std::vector<UTF8_One> MakeInputLine() {
	std::vector<UTF8_One> line;

	std::string input;
	while (true) {
		std::getline(std::cin, input);
		if (!input.empty()) {
			break;
		}
	}


	for (int i = 0; i < input.length();) {
		UTF8_One U = ParseOne(input.c_str(), i, input.length());
		line.push_back(U);
		i += U.Length + 1;
	}
	return line;
}


//not complete.

int main() {
	DosWindow W = DosWindow();


	do {
		auto line = MakeInputLine();
		W.WriteLine(0, line);
		
	} while (W.Update(W.corutine));

	return 0;
}
