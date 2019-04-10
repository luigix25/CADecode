#include "decode.h"

uint8_t Decode::getFormat(){
	return (regs.opcode & 0xE0) >> 5;
}

uint8_t Decode::getID(){
	return (regs.opcode & 0x1F);
}

message* Decode::create_message_for_fetch(){

	message *m = new message;
	strcpy(m->source,DECODE);
	strcpy(m->dest,FETCH);
	m->magic_struct = NULL;
	m->next = NULL;

	return m;


}


message* Decode::create_message_for_ALU(){

	message *m = new message;
	strcpy(m->source,DECODE);
	strcpy(m->dest,ALU);
	m->magic_struct = NULL;
	m->next = NULL;

	return m;
}


message* Decode::create_message_for_memory(bool type,uint16_t address,uint16_t data=0){
	
	message *m = new message;
	strcpy(m->source,DECODE);
	strcpy(m->dest,MEMORY);

	shared_dec_mem.type = type;
	shared_dec_mem.address = address;
	shared_dec_mem.data = data;

	m->magic_struct = (void*)&shared_dec_mem;
	m->next = NULL;

	return m;
}


void Decode::onNotify(message *msg){

	if(strcmp(msg->dest,DECODE) != 0){
		return;
	}

	if(strcmp(msg->source,MEMORY)== 0){
		sendWithDelay(handle_memory(msg),EVENT_TIME);
	}

	if(strcmp(msg->source,FETCH)== 0){
		 sendWithDelay(handle_fetch(msg),EVENT_TIME);
	}

	if(strcmp(msg->source,ALU)== 0){
		sendWithDelay(handle_alu(msg),EVENT_TIME);
	} 

	return; 				//cannot reach here
}


message* Decode::handle_memory(message *msg){
	if(msg->magic_struct == NULL){	//ack
		return create_message_for_fetch();	//???	
	}
	else{
		uint16_t data = *((uint16_t*)msg->magic_struct);
		return handle_load(data);										// può essere solamente una LOAD		
	}
}


message* Decode::handle_load(uint16_t data){

	global_regs.general_regs[regs.dest] = data;
	return create_message_for_fetch();

}


message* Decode::handle_alu(message *msg){
/*
	queste cose non servono più perchè non abbiamo indirizzamenti indiretti nell'instruction set.

	if(opcode == op_in_mem){
		uint16_t dest_for_cache = dest;	//è il registro di destinazione fissato durante la fase di fetch
		uint16_t data = reg_mem;
		send_msg_to_cache(type = 1,dest_for_cache,data);	//è per forza scrittura
	}
	else{
		if(!check_flag())
			handle_error(); */
		return create_message_for_fetch();
	//}
}


message* Decode::handle_fetch(message *msg){

	uint8_t format = getFormat();

	switch(format){
		case 0:
			return format_0();
			break;
		case 1:
			return format_1();
			break;
		case 2:
			return format_2();
			break;
		case 3:
			return format_3();
			break;
		case 4:
			return format_4();
			break;
		default:	
			cerr<<"Unknown Format"<<endl;
			exit(-1);
	}

	/*if(prima_operazione == nessun_modulo)		//operazioni tregs.ipo mov reg/imm reg,hlt,nop,jump 
		execute_tregs.ipo_nessun_modulo();	
	else if (prima_operazione == modulo_memoria)	//TUTTE le operazioni per le quali una lettura in memoria è sempre necessaria oppure operazioni che
		execute_tregs.ipo_modulo_memoria();				//hanno come destinazione un indirizzo di memoria
													//ESEMPI: primo gruppo:mov imm/reg mem/(reg) ---- 

															  gruppo due:op.ALU 	reg,MEM 	
																					MEM,reg
																					(reg),reg
																					reg,(reg)
																					imm,(reg)
																gruppo tre: mv mem,reg
																			mv (reg),reg




	else if(prima_operazione == modulo_alu)			//essenzialmente saranno sempre e solo operazioni che terminano subito senza passare dalla memoria
		execute_tregs.ipo_modulo_alu();					//ESEMPI: sub reg,reg sub imm,reg add immg,reg ecc.
*/
}

/*
function execute_tregs.ipo_modulo_memoria(){
	if(opcode == gruppo uno){							//sono operazioni di scrittura in memoria
		//prepara dati per cache						//significa che l'operazione da fare termina subito dopo aver fatto la store con la cache
		send_msg_to_cache(type = 1,dest_for_cache,data);			//ogni scrittura in memoria che viene fatta da qui è una operazione single step.
	}												//pertanto i tregs.ipi possibili sono solo i 4 appartenenti al gruppo uno


	else if(opcode == gruppo due || gruppo tre){				
													//sono operazioni di lettura in memoria che possono terminare senza andare dalla alu (gruppo 3)
													//oppure possono andare alla alu (gruppo 2)
													//in ogni caso non possono terminare con un solo passaggio
		//prepara dati per cache distinguendo
		//indirizzamento indiretto o di memoria
		send_msg_to_cache(type = 0, dest_for_cache);


	}
}*/


//FORMATI

message* Decode::format_0(){
	uint8_t id = getID();
	switch(id){
		case 0:		//HLT
			exit(1);
			break;
		case 1:		//NOP
			return create_message_for_fetch();
			break;
		default:
			cerr<<"Unknown Instruction Format 0"<<endl;
			exit(-1);
	}
}


message* Decode::format_1(){
	uint8_t id = getID();
	switch(id){
		case 0: case 10:		//JE, JZ
			if(extract_flag(ZF))
			{
				regs.ip = regs.source;
			}			
			break;
		case 1: case 11:		//JNE, JNZ
			if(!extract_flag(ZF))
			{
				regs.ip = regs.source;
			}	
			break;
		case 2:			//JA
			if(!extract_flag(CF) && !extract_flag(ZF))
			{
				regs.ip = regs.source;
			}
			break;
		case 3: case 13:		//JAE, JNC
			if(!extract_flag(CF))
			{
				regs.ip = regs.source;
			}
			break;
		case 4: case 12:		//JB, JC
			if(extract_flag(CF))
			{
				regs.ip = regs.source;
			}
			break;
		case 5:			//JBE
			if(extract_flag(CF) || extract_flag(ZF))
			{
				regs.ip = regs.source;
			}
			break;
		case 6:			//JG
			if(!extract_flag(ZF) && (extract_flag(SF) == extract_flag(OF)))
			{
				regs.ip = regs.source;
			}
			break;
		case 7:			//JGE
			if(extract_flag(SF) == extract_flag(OF))
			{
				regs.ip = regs.source;
			}
			break;
		case 8:			//JL
			if(extract_flag(SF) != extract_flag(OF))
			{
				regs.ip = regs.source;
			}
			break;
		case 9:			//JLE
			if(extract_flag(ZF) || (extract_flag(SF) != extract_flag(OF)))
			{
				regs.ip = regs.source;
			}
			break;
		case 14:		//JO
			if(extract_flag(OF))
			{
				regs.ip = regs.source;
			}
			break;
		case 15:		//JNO
			if(!extract_flag(OF))
			{
				regs.ip = regs.source;
			}
			break;
		case 16:		//JS
			if(extract_flag(SF))
			{
				regs.ip = regs.source;
			}
			break;
		case 17:		//JNS
			if(!extract_flag(SF))
			{
				regs.ip = regs.source;
			}
			break;
		default:
			cerr<<"Unknown Instruction Format 1"<<endl;
			exit(-1);
	}

	return create_message_for_fetch();
}


message* Decode::format_2(){
	uint8_t id = getID();

	switch(id){
		case 0: case 10:		//JE, JZ
			if(extract_flag(ZF))
			{
				regs.ip = global_regs.general_regs[regs.source];
			}			
			break;
		case 1: case 11:		//JNE, JNZ
			if(!extract_flag(ZF))
			{
				regs.ip = global_regs.general_regs[regs.source];
			}	
			break;
		case 2:			//JA
			if(!extract_flag(CF) && !extract_flag(ZF))
			{
				regs.ip = global_regs.general_regs[regs.source];
			}
			break;
		case 3: case 13:		//JAE, JNC
			if(!extract_flag(CF))
			{
				regs.ip = global_regs.general_regs[regs.source];
			}
			break;
		case 4: case 12:		//JB, JC
			if(extract_flag(CF))
			{
				regs.ip = global_regs.general_regs[regs.source];
			}
			break;
		case 5:			//JBE
			if(extract_flag(CF) || extract_flag(ZF))
			{
				regs.ip = global_regs.general_regs[regs.source];
			}
			break;
		case 6:			//JG
			if(!extract_flag(ZF) && (extract_flag(SF) == extract_flag(OF)))
			{
				regs.ip = global_regs.general_regs[regs.source];
			}
			break;
		case 7:			//JGE
			if(extract_flag(SF) == extract_flag(OF))
			{
				regs.ip = global_regs.general_regs[regs.source];
			}
			break;
		case 8:			//JL
			if(extract_flag(SF) != extract_flag(OF))
			{
				regs.ip = global_regs.general_regs[regs.source];
			}
			break;
		case 9:			//JLE
			if(extract_flag(ZF) || (extract_flag(SF) != extract_flag(OF)))
			{
				regs.ip = global_regs.general_regs[regs.source];
			}
			break;
		case 14:		//JO
			if(extract_flag(OF))
			{
				regs.ip = global_regs.general_regs[regs.source];
			}
			break;
		case 15:		//JNO
			if(!extract_flag(OF))
			{
				regs.ip = global_regs.general_regs[regs.source];
			}
			break;
		case 16:		//JS
			if(extract_flag(SF))
			{
				regs.ip = global_regs.general_regs[regs.source];
			}
			break;
		case 17:		//JNS
			if(!extract_flag(SF))
			{
				regs.ip = global_regs.general_regs[regs.source];
			}
			break;
		case 18 ... 21 :						//INC, DEC, NEG, NOT
			alu_regs.operand1 = global_regs.general_regs[regs.source];		    //registro per la ALU dove troverà il primo operando
			alu_regs.operand2 = 0;					    //controllare se serve
			alu_regs.destination_reg = regs.source;
			alu_regs.opcode = regs.opcode;
			return create_message_for_ALU();
			break;
		
		default:
			cerr<<"Unknown Instruction Format 2"<<endl;
			exit(-1);
	}

	return create_message_for_fetch();
}


message* Decode::format_3(){
	uint8_t id = getID();

	switch(id){
		case 0:								//MOV immediato, registro
			global_regs.general_regs[regs.dest] = regs.source;
			return create_message_for_fetch();
			break;
		case 1 ... 13 :		// OP immediato, registro			
			alu_regs.operand1 = regs.source;		   
			alu_regs.operand2 = global_regs.general_regs[regs.dest];
			alu_regs.destination_reg = regs.dest;
			alu_regs.opcode = regs.opcode;
			return create_message_for_ALU();
			break;
		case 14: 							//LOAD immediato, registro
			return create_message_for_memory(0, regs.source);
			break;
		case 15:							//STORE immediato, registro  (in source trovo il dato, in dest trovo l'indirizzo)
			return create_message_for_memory(1,global_regs.general_regs[regs.dest],regs.source);		//Normalmente sarebbe dovuto essere il contrario, ma in fase di fetching l'indirizzo di 												//memoria in cui memorizzare il dato viene passato nel registro source e non nel registro 													//dest; per questo motivo abbiamo i registri invertiti nell'inviare il messaggio
			break;
		default:
			cerr<<"Unknown Instruction Format 3"<<endl;
			exit(-1);
	}
}


message* Decode::format_4(){
	uint8_t id = getID();

	switch(id){
		case 0:								//MOV registro, registro
			global_regs.general_regs[regs.dest] = global_regs.general_regs[regs.source];
			return create_message_for_fetch();	
			break;
		case 1 ... 13 :						// OP registro, registro			
			alu_regs.operand1 = global_regs.general_regs[regs.source];		   
			alu_regs.operand2 = global_regs.general_regs[regs.dest];
			alu_regs.destination_reg = regs.dest;
			alu_regs.opcode = regs.opcode;
			return create_message_for_ALU();
			break;
		case 14: 							//LOAD registro, registro
			return create_message_for_memory(0, global_regs.general_regs[regs.source]);
			break;
		case 15:							//STORE registro, registro (in source trovo il dato, in dest trovo l'indirizzo)
			return create_message_for_memory(1, global_regs.general_regs[regs.dest],global_regs.general_regs[regs.source]);
			break;
		case 16: 							//XCHG resgistro registro
		{
			uint16_t tmp = global_regs.general_regs[regs.source];
			global_regs.general_regs[regs.source] = global_regs.general_regs[regs.dest];
			global_regs.general_regs[regs.dest] = tmp;
			return create_message_for_fetch();
			break;
		}
		default:
			cerr<<"Unknown Instruction Format 4"<<endl;
			exit(-1);
	}
}


bool Decode::extract_flag(uint8_t index){
	return (global_regs.flag >> index) &1;

}
