// ------------------------------------------------------------------
// Elf 2D engine - a game development framework
// ------------------------------------------------------------------

#ifndef __ELF_H_INCLUDED__
#define __ELF_H_INCLUDED__

#if !defined WIN32
#error "Only Windows supported"
#endif

#ifdef ELF2D_EXPORTS
#define Elf2D_EXPORTED __declspec(dllexport)
#else
#define Elf2D_EXPORTED __declspec(dllimport)
#endif

extern "C"
{

// Keyboard keys
enum ElfKey
{
	ElfKey_Escape          = 0x01,
	ElfKey_D1              = 0x02,
	ElfKey_D2              = 0x03,
	ElfKey_D3              = 0x04,
	ElfKey_D4              = 0x05,
	ElfKey_D5              = 0x06,
	ElfKey_D6              = 0x07,
	ElfKey_D7              = 0x08,
	ElfKey_D8              = 0x09,
	ElfKey_D9              = 0x0A,
	ElfKey_D0              = 0x0B,
	ElfKey_Minus           = 0x0C,    // Main keyboard
	ElfKey_Equals          = 0x0D,
	ElfKey_Back            = 0x0E,    // Backspace
	ElfKey_Tab             = 0x0F,
	ElfKey_Q               = 0x10,
	ElfKey_W               = 0x11,
	ElfKey_E               = 0x12,
	ElfKey_R               = 0x13,
	ElfKey_T               = 0x14,
	ElfKey_Y               = 0x15,
	ElfKey_U               = 0x16,
	ElfKey_I               = 0x17,
	ElfKey_O               = 0x18,
	ElfKey_P               = 0x19,
	ElfKey_LeftBracket     = 0x1A,
	ElfKey_RightBracket    = 0x1B,
	ElfKey_Return          = 0x1C,    // Enter (main keyboard)
	ElfKey_LeftControl     = 0x1D,
	ElfKey_A               = 0x1E,
	ElfKey_S               = 0x1F,
	ElfKey_D               = 0x20,
	ElfKey_F               = 0x21,
	ElfKey_G               = 0x22,
	ElfKey_H               = 0x23,
	ElfKey_J               = 0x24,
	ElfKey_K               = 0x25,
	ElfKey_L               = 0x26,
	ElfKey_Semicolon       = 0x27,
	ElfKey_Apostrophe      = 0x28,
	ElfKey_Grave           = 0x29,    // Accent grave
	ElfKey_LeftShift       = 0x2A,
	ElfKey_Backslash       = 0x2B,
	ElfKey_Z               = 0x2C,
	ElfKey_X               = 0x2D,
	ElfKey_C               = 0x2E,
	ElfKey_V               = 0x2F,
	ElfKey_B               = 0x30,
	ElfKey_N               = 0x31,
	ElfKey_M               = 0x32,
	ElfKey_Comma           = 0x33,
	ElfKey_Period          = 0x34,    // . (main keyboard)
	ElfKey_Slash           = 0x35,    // / (main keyboard)
	ElfKey_RightShift      = 0x36,
	ElfKey_Multiply        = 0x37,    // * (numeric keypad)
	ElfKey_LeftMenu        = 0x38,    // Left ALT
	ElfKey_Space           = 0x39,
	ElfKey_Capital         = 0x3A,
	ElfKey_F1              = 0x3B,
	ElfKey_F2              = 0x3C,
	ElfKey_F3              = 0x3D,
	ElfKey_F4              = 0x3E,
	ElfKey_F5              = 0x3F,
	ElfKey_F6              = 0x40,
	ElfKey_F7              = 0x41,
	ElfKey_F8              = 0x42,
	ElfKey_F9              = 0x43,
	ElfKey_F10             = 0x44,
	ElfKey_NumLock         = 0x45,
	ElfKey_Scroll          = 0x46,    // Scroll lock
	ElfKey_NumPad7         = 0x47,
	ElfKey_NumPad8         = 0x48,
	ElfKey_NumPad9         = 0x49,
	ElfKey_Subtract        = 0x4A,    // - (numeric keypad)
	ElfKey_NumPad4         = 0x4B,
	ElfKey_NumPad5         = 0x4C,
	ElfKey_NumPad6         = 0x4D,
	ElfKey_Add             = 0x4E,    // + (numeric keypad)
	ElfKey_NumPad1         = 0x4F,
	ElfKey_NumPad2         = 0x50,
	ElfKey_NumPad3         = 0x51,
	ElfKey_NumPad0         = 0x52,
	ElfKey_Decimal         = 0x53,    // . (numeric keypad)	
	ElfKey_F11             = 0x57,
	ElfKey_F12             = 0x58,
	ElfKey_NumPadEnter     = 0x9C,    // Enter (numeric keypad)
	ElfKey_RightControl    = 0x9D,
	ElfKey_NumPadComma     = 0xB3,    // , (numeric keypad)
	ElfKey_Divide          = 0xB5,    // / (numeric keypad)
	ElfKey_SysRq           = 0xB7,
	ElfKey_RightMenu       = 0xB8,    // Right ALT
	ElfKey_Pause           = 0xC5,    // Pause
	ElfKey_Home            = 0xC7,    // Home (arrow keypad)
	ElfKey_Up              = 0xC8,    // UpArrow (arrow keypad)
	ElfKey_Prior           = 0xC9,    // PgUp (arrow keypad)
	ElfKey_Left            = 0xCB,    // LeftArrow (arrow keypad)
	ElfKey_Right           = 0xCD,    // RightArrow (arrow keypad)
	ElfKey_End             = 0xCF,    // End (arrow keypad)
	ElfKey_Down            = 0xD0,    // DownArrow (arrow keypad)
	ElfKey_Next            = 0xD1,    // PgDn (arrow keypad)
	ElfKey_Insert          = 0xD2,    // Insert (arrow keypad)
	ElfKey_Delete          = 0xD3,    // Delete (arrow keypad)
	
	//  Alternate names for keys
	ElfKey_Backspace       = ElfKey_Back,            // Backspace
	ElfKey_NumPadstar      = ElfKey_Multiply,        // * (numeric keypad)
	ElfKey_LeftAlt         = ElfKey_LeftMenu,        // Left ALT
	ElfKey_CapsLock        = ElfKey_Capital,         // CapsLock
	ElfKey_NumPadMinus     = ElfKey_Subtract,        // - (numeric keypad)
	ElfKey_NumPadPlus      = ElfKey_Add,             // + (numeric keypad)
	ElfKey_NumPadPeriod    = ElfKey_Decimal,         // . (numeric keypad)
	ElfKey_NumPadSlash     = ElfKey_Divide,          // / (numeric keypad)
	ElfKey_RightAlt        = ElfKey_RightMenu,       // Right ALT
	ElfKey_UpArrow         = ElfKey_Up,              // UpArrow (arrow keypad)
	ElfKey_PageUp          = ElfKey_Prior,           // PgUp (arrow keypad)
	ElfKey_LeftArrow       = ElfKey_Left,            // LeftArrow (arrow keypad)
	ElfKey_RightArrow      = ElfKey_Right,           // RightArrow (arrow keypad)
	ElfKey_DownArrow       = ElfKey_Down,            // DownArrow (arrow keypad)
	ElfKey_PageDown        = ElfKey_Next,            // PgDn (arrow keypad)
};

// Texture wrap modes
enum ElfTextureWrapMode
{
	ElfTWM_Wrap	= 0,
	ElfTWM_Clamp
};

// Blend modes
enum ElfBlendMode
{
	ElfBM_Zero			= 1,
	ElfBM_One,
	ElfBM_SrcColor,
	ElfBM_InvSrcColor,
	ElfBM_SrcAlpha,
	ElfBM_InvSrcAlpha,
	ElfBM_DestAlpha,
	ElfBM_InvDestAlpha,
	ElfBM_DestColor,
	ElfBM_InvDestColor
};

// Log modes
enum ElfLogMode
{
	ElfLM_None			= 1,
	ElfLM_STDOut,
	ElfLM_Console,
	ElfLM_File		
};



// Configuration
// Call these before elfSystem_Initialise
typedef void (*TCallbackFunction1)(const float); // Param - deltaTime in seconds
typedef void (*TCallbackFunction2)();

Elf2D_EXPORTED void elfConfig_SetWindowParams(int width, int height, bool windowed, const char* title);

Elf2D_EXPORTED void elfConfig_SetUpdateCallback(TCallbackFunction1 function);
Elf2D_EXPORTED void elfConfig_SetRenderCallback(TCallbackFunction1 function);
Elf2D_EXPORTED void elfConfig_SetInitialiseCallback(TCallbackFunction2 function);
Elf2D_EXPORTED void elfConfig_SetShutdownCallback(TCallbackFunction2 function);

Elf2D_EXPORTED void elfConfig_SetSystemLogMode(ElfLogMode logMode, const char* fileName = "elf2d.log.txt");



// System
Elf2D_EXPORTED bool elfSystem_Initialise();				// Initialises the framework. Configure before calling this.	
Elf2D_EXPORTED void elfSystem_Run();					// Runs the main loop	
Elf2D_EXPORTED void elfSystem_Shutdown();				// Shuts down the framework
Elf2D_EXPORTED const char* elfSystem_GetLastError();	// Gets the description of the last error

Elf2D_EXPORTED void elfSystem_Exit();					// Triggers the shutdown



// Mouse cursors (hardware)
Elf2D_EXPORTED bool elfCursor_Load(const char* id, const char* fileName);
Elf2D_EXPORTED int  elfCursor_GetId(const char* id);
Elf2D_EXPORTED void elfCursor_Unload(int id);
Elf2D_EXPORTED void elfCursor_Bind(int id);


// Video
Elf2D_EXPORTED void elfVideo_Clear(unsigned long colour);
Elf2D_EXPORTED void elfVideo_ClearComponents(unsigned char a, unsigned char r, unsigned char g, unsigned char b);

Elf2D_EXPORTED void elfVideo_EnableBlending(bool enable);
Elf2D_EXPORTED void elfVideo_SetBlendMode(ElfBlendMode source, ElfBlendMode destination);



// Textures
Elf2D_EXPORTED bool elfTexture_Load(const char* id, const char* fileName);
Elf2D_EXPORTED int  elfTexture_GetId(const char* id);
Elf2D_EXPORTED void elfTexture_Unload(int id);
Elf2D_EXPORTED void elfTexture_Bind(int id, ElfTextureWrapMode wrapMode = ElfTWM_Clamp);

Elf2D_EXPORTED bool elfTexture_Create(const char* id, unsigned int width, unsigned int height);
Elf2D_EXPORTED void elfTexture_Update(int id, const unsigned int* data);

Elf2D_EXPORTED bool elfTexture_GetSize(int id, unsigned int& width, unsigned int& height);



// Quads
//
// Point order:
//  0      1
//   |----|
//   |    |
//   |----|
//  3      2
//
Elf2D_EXPORTED void elfQuads_begin();
Elf2D_EXPORTED void elfQuads_end();
// Only elfQuads_* calls can be made between elfQuads_begin and elfQuads_end.

Elf2D_EXPORTED void elfQuads_setColour(unsigned long colour);	
Elf2D_EXPORTED void elfQuads_setTextureRegion(const float coords[8]);
Elf2D_EXPORTED void elfQuads_setTextureRectangle(const float& x, const float& y, const float& w, const float& h);
Elf2D_EXPORTED void elfQuads_setTextureRegionPixels(const float coords[8]);
Elf2D_EXPORTED void elfQuads_setTextureRectanglePixels(const float& x, const float& y, const float& w, const float& h);

Elf2D_EXPORTED void elfQuads_setShapeRotation(const float& degrees);
Elf2D_EXPORTED void elfQuads_setShapeRotationRad(const float& radians);

Elf2D_EXPORTED void elfQuads_drawShape(const float coords[8]);
Elf2D_EXPORTED void elfQuads_drawShapeOffset(const float& x, const float& y, const float coords[8]);
Elf2D_EXPORTED void elfQuads_drawRectangle(const float& x, const float& y, const float& w, const float& h);
Elf2D_EXPORTED void elfQuads_drawRectangleCentered(const float& x, const float& y, const float& w, const float& h);



// Audio
Elf2D_EXPORTED bool elfSoundEffect_Load(const char* id, const char* fileName);
Elf2D_EXPORTED void elfSoundEffect_Unload(int id);
Elf2D_EXPORTED int  elfSoundEffect_GetId(const char* id);
Elf2D_EXPORTED void elfSoundEffect_Fire(int id, const float volume = 1.0f, const float pan = 0.0f);
	


// Input
// Mouse handles 8 keys
// Keyboard handles 256 keys 
Elf2D_EXPORTED bool elfMouse_IsButtonPressed(unsigned char key);	// Was the button pressed this frame
Elf2D_EXPORTED bool elfMouse_IsButtonReleased(unsigned char key);	// Was the button released this frame
Elf2D_EXPORTED bool elfMouse_IsButtonDown(unsigned char key);		// Was the button down this frame

Elf2D_EXPORTED long elfMouse_GetWheel();							// Mouse wheel delta. Expect this to be a multiple of 120

Elf2D_EXPORTED void elfMouse_SetPos(long x, long y);
Elf2D_EXPORTED void elfMouse_GetPos(long& x, long& y);
Elf2D_EXPORTED void elfMouse_GetPosDelta(long& x, long &y);			// Position delta since last frame

Elf2D_EXPORTED bool elfKeyboard_IsKeyPressed(unsigned char key);	// Was the key pressed this frame
Elf2D_EXPORTED bool elfKeyboard_IsKeyReleased(unsigned char key);	// Was the key released this frame
Elf2D_EXPORTED bool elfKeyboard_IsKeyDown(unsigned char key);		// Was the key down this frame



// Network
typedef int ElfClientId;
typedef unsigned short ElfMessageId;

// Common functions
// These can only be used in the following callbacks:
// * Server - OnReceiveCallback
// * Client - OnReceiveCallback
Elf2D_EXPORTED bool elfMessage_readFloat(float& value);
Elf2D_EXPORTED bool elfMessage_readInt(int& value);
Elf2D_EXPORTED bool elfMessage_readShort(short& value);
Elf2D_EXPORTED bool elfMessage_readBytes(char* buffer, const int length);
// These can only be used in between:
// * Server - beginSendToAll/beginSend and endSend
// * Client - beginSend/endSend
Elf2D_EXPORTED void elfMessage_writeFloat(const float& value);
Elf2D_EXPORTED void elfMessage_writeInt(const int& value);
Elf2D_EXPORTED void elfMessage_writeShort(const short& value);
Elf2D_EXPORTED void elfMessage_writeBytes(const char* buffer, const int length);

// Server functions
Elf2D_EXPORTED bool elfServer_start(unsigned short port);
Elf2D_EXPORTED void elfServer_stop();
Elf2D_EXPORTED void elfServer_forceDisconnectClient(ElfClientId id);
Elf2D_EXPORTED void elfServer_forceDisconnectAllClients();

Elf2D_EXPORTED void elfServer_beginSendToAll(ElfMessageId msgId);
Elf2D_EXPORTED void elfServer_beginSend(ElfClientId clientId, ElfMessageId msgId);
Elf2D_EXPORTED void elfServer_endSend();

// Server callbacks
typedef void (*TServerCallbackFunction1)(ElfClientId);
typedef void (*TServerCallbackFunction2)(ElfClientId, ElfMessageId);

Elf2D_EXPORTED void elfServer_setOnClientConnectCallback(TServerCallbackFunction1 function);
Elf2D_EXPORTED void elfServer_setOnClientDisconnectCallback(TServerCallbackFunction1 function);
Elf2D_EXPORTED void elfServer_setOnReceiveCallback(TServerCallbackFunction2 function);

// Client functions
Elf2D_EXPORTED bool elfClient_connect(unsigned short port, const char* ip);
Elf2D_EXPORTED void elfClient_disconnect();

Elf2D_EXPORTED void elfClient_beginSend(ElfMessageId msgId);
Elf2D_EXPORTED void elfClient_endSend();

// Client callbacks
typedef void (*TClientCallbackFunction1)(bool);
typedef void (*TClientCallbackFunction2)();
typedef void (*TClientCallbackFunction3)(ElfMessageId);

Elf2D_EXPORTED void elfClient_setOnConnectCallback(TClientCallbackFunction1 function);
Elf2D_EXPORTED void elfClient_setOnDisconnectCallback(TClientCallbackFunction2 function);
Elf2D_EXPORTED void elfClient_setOnReceiveCallback(TClientCallbackFunction3 function);

};

#endif // __ELF2D_H_INCLUDED__
