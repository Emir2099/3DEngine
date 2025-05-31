#pragma once
#include <windows.h> // Ensure this is first
#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;    // Window class is a friend of Keyboard to allow it to access private members (friend in need is a friend indeed)
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event()
			:
			type( Type::Invalid ),
			code( 0u )
		{}
		Event( Type type,unsigned char code ) noexcept
			:
			type( type ),
			code( code )
		{}
		bool IsPress() const noexcept
		{
			return type == Type::Press;
		}
		bool IsRelease() const noexcept
		{
			return type == Type::Release;
		}
		bool IsValid() const noexcept
		{
			return type != Type::Invalid;
		}
		unsigned char GetCode() const noexcept
		{
			return code;
		}
	};
public:
	Keyboard() = default;
	Keyboard( const Keyboard& ) = delete;
	Keyboard& operator=( const Keyboard& ) = delete;
	// key event stuff
	bool KeyIsPressed( unsigned char keycode ) const noexcept;
	Event ReadKey() noexcept;    // ReadKey pulls an event from the event queue
	bool KeyIsEmpty() const noexcept;       // Checks if there is an event in the event queue
	void FlushKey() noexcept;      // FlushKey clears the key event queue
	// char event stuff
	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;    // FlushChar clears the char event queue
	void Flush() noexcept;    // Flush clears both the key and char event queues
	// autorepeat control
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutorepeatIsEnabled() const noexcept;
private:   // these are used by the Window class to communicate with the keyboard (implement that by making the Window class a friend of Keyboard)
	void OnKeyPressed( unsigned char keycode ) noexcept;
	void OnKeyReleased( unsigned char keycode ) noexcept;
	void OnChar( char character ) noexcept;
	void ClearState() noexcept;  // clears the bitset (given below)
	template<typename T>
	static void TrimBuffer( std::queue<T>& buffer ) noexcept;    // removes ites from the queue if it exceeds the buffer size
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};