#ifndef EXO_H
#define EXO_H
  
//       _      __       __                       _       
//    __| |___ / _|___  / _|___    __ ___ _ _  __| |_ ___ 
//   / _` / -_)  _(_-<  > _|_ _|  / _/ _ \ ' \(_-<  _(_-< 
//   \__,_\___|_| /__/  \_____|   \__\___/_||_/__/\__/__/ 
//                                                        
#define EXO_MSG_NONE 0

//    _                     _      __    
//   | |_ _  _ _ __  ___ __| |___ / _|___
//   |  _| || | '_ \/ -_) _` / -_)  _(_-<
//    \__|\_, | .__/\___\__,_\___|_| /__/
//        |__/|_|                        
typedef exo_msg_type_t uint16_t;

//       _               _      
//    __| |_ _ _ _  _ __| |_ ___
//   (_-<  _| '_| || / _|  _(_-<
//   /__/\__|_|  \_,_\__|\__/__/
//                              
typedef struct {
	exo_msg_type_t type;
	uint32_t       length;
} exo_msg_hdr_t;

typedef struct {
	exo_msg_hdr_t hdr;
	void*         payload;
} exo_msg_t;

typedef struct {
	int severity; // larger is worse
	char origin[32];
	char description[256];
} exo_msg_fault_t;

typedef struct {
	uint32_t magic;
	uint32_t chksum;
	uint8_t  msg_count;
} exo_envolope_hdr_t;

typedef struct {
	exo_envolope_hdr_t hdr;
	exo_msg_t*         msgs;
} exo_envolope_t;

typedef struct {
	struct {
		int            (*enqueue)(exo_msg_t msg);
		exo_msg_type_t (*peek_next)();
		int            (*dequeue)(exo_msg_t* msg);
	} msg;
	int (*config)(exo_mod_t* mod);
	int (*entry)(exo_mod_t* mod);
	int (*update)(exo_mod_t* mod);
	int (*exit)(exo_mod_t* mod);
	int (*fault)(exo_mod_t* mod, exo_msg_fault_t fault); 
} exo_mod_t;

//     __              _   _             
//    / _|_  _ _ _  __| |_(_)___ _ _  ___
//   |  _| || | ' \/ _|  _| / _ \ ' \(_-<
//   |_|  \_,_|_||_\__|\__|_\___/_||_/__/
//                                       
int exo_free_envolope(exo_envolope_t* env); // TODO: determine true need

#endif
