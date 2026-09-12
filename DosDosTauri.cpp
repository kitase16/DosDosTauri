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
#include <limits>

//i lose by human engineering, but i win by human engineering not

namespace fs = std::filesystem;

using F16 = float;

template<typename T>
class Rational {
public:	
	Rational(T a, T b) : A(a), B(b) {}
	Rational(T N) : A(N), B(1) {}
	Rational(float N) : A(N), B(1) {}
	Rational(Rational<T>& In) : A(In.A), B(In.B) {}

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

	template<class R>
	R Divaide() {
		//Denomi();
		return A / static_cast<float>(B);
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
		data.resize(width.Divaide<float>() * height.Divaide<float>());
	}
	T area() const {
		return width * height;
	}

	bool SetPixel(T x, T y, T value) {
		if (x.Divaide<float>() < 0 || x.Divaide<float>() >= width.Divaide<float>() || y.Divaide<float>() < 0 || y.Divaide<float>() >= height.Divaide<float>()) {
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

struct MemoryAllocater_asBits;

struct EntarTainmentLayer {
	Surface<Rational<std::intmax_t>> surface,ZBuffer;
	Palse<Rational<std::intmax_t>> palse;
	EntarTainmentLayer(Rational<std::intmax_t> width, Rational<std::intmax_t> height, std::intmax_t palse_size) : surface({ width }, { height }), palse(palse_size) {
		palse.data.resize(static_cast<std::size_t>(width.Divaide<float>() * height.Divaide<float>()));
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


struct MemoryAllocater_AsBits{
	MemoryAllocater_AsBits() = default;
	MemoryAllocater_AsBits(std::size_t size) {
		buffer.resize(size);
	}
	
	struct MemoryHolder_asBit {
		char Tag = 'B';
		std::size_t BaseAddress = 0;
		std::size_t Length = 0;
		MemoryAllocater_AsBits* Parent;

		bool Index(std::size_t offset) {
			if (offset >= Length) {
				throw std::out_of_range("Offset is out of range.");
			}
			return *(Parent->buffer.data() + (BaseAddress/8)+ (offset/8) + (offset % 8)) & (1 << (offset % 8));
		}

		void offset(std::size_t offset, bool value) {
			if (offset >= Length) {
				throw std::out_of_range("Offset is out of range.");
			}
			if (value) {
				*(Parent->buffer.data() + (BaseAddress / 8) + (offset / 8) + (offset % 8)) |= (1 << (offset % 8));
			}
			else {
				*(Parent->buffer.data() + (BaseAddress / 8) + (offset / 8) + (offset % 8)) &= ~(1 << (offset % 8));
			}
		}


	};
	
	std::vector<std::uint8_t> buffer;
	std::vector<std::shared_ptr<MemoryHolder_asBit>> holders;

	std::shared_ptr<MemoryHolder_asBit> Allocate(std::size_t length) {
		std::size_t address = 0;
		for (const auto& holder : holders) {
			if (address + length <= holder->BaseAddress) {
				break;
			}
			address = holder->BaseAddress + holder->Length;
		}
		if (address + length > buffer.size()) {
			buffer.resize(address + length);
		}
		auto newHolder = std::make_shared<MemoryHolder_asBit>();
		newHolder->BaseAddress = address;
		newHolder->Length = length;
		newHolder->Parent = this;
		holders.push_back(newHolder);
		return newHolder;
	}

	bool free(std::shared_ptr<MemoryHolder_asBit> holder) {
		auto it = std::find(holders.begin(), holders.end(), holder);
		if (it != holders.end()) {
			holders.erase(it);
			return true;
		}
		return false;
	}

	bool AllocBaseByte(std::size_t length) {
		buffer.resize(length);
		return true;
	}

	std::size_t Size() const {
		return buffer.size();
	}

	std::size_t HolderCount() const {
		return holders.size();
	}

	std::shared_ptr<MemoryHolder_asBit> operator[](std::size_t index) {
		if (index >= holders.size()) {
			throw std::out_of_range("Index is out of range.");
		}
		return holders[index];
	}
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

	std::uint8_t Length = 0;
	std::uint8_t Char[4] = { 0,0,0,'\0'};

	operator char* () {
		return reinterpret_cast<char*>(Char);
	}
};

UTF8_One ParseOne(const char* P, int I, size_t L) {
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
	DosWindow() : layer(80, 25, 2000), screen{ 80, 25 }, Back{ 80, 25 }, Memory(1024){
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
		for (std::intmax_t x = 0; x < line.size() && x < static_cast<std::intmax_t>(screen.width); ++x) {
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
	MemoryAllocater_AsBits Memory;	
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

	if (input.length() > static_cast<size_t>(std::numeric_limits<int>::max())) {
	    // エラー処理（長すぎる等)
	}
	for (int i = 0; i < input.length();) {
		UTF8_One U = ParseOne(input.c_str(), i, static_cast<int>(input.length()));
		line.push_back(U);
		i += U.Length + 1;
	}
	return line;
}


//not complete.
/**/
int main() {
	DosWindow W = DosWindow();


	do {
		auto line = MakeInputLine();
		W.WriteLine(0, line);
		
	} while (W.Update(W.corutine));

	return 0;
}
/** /

int main() {
	DosWindow W = DosWindow();
	// Example usage of the DosWindow
	W.SetCharcter(0, 0, UTF8_One{ 'H', { 0, 0, 0, 0 } });
	W.SetCharcter(1, 0, UTF8_One{ 'e', { 0, 0, 0, 0 } });
	W.SetCharcter(2, 0, UTF8_One{ 'l', { 0, 0, 0, 0 } });
	W.SetCharcter(3, 0, UTF8_One{ 'l', { 0, 0, 0, 0 } });
	W.SetCharcter(4, 0, UTF8_One{ 'o', { 0, 0, 0, 0 } });
	// Update the window to render the changes
	W.Update(W.corutine);
	return 0;
}
/**/