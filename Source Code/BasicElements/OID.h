#ifndef _OID
#define _OID



class OID{
	unsigned char data[4];
public:
	OID(){
		for( int i=0;i<4;i++)data[i]=0;	
	}

	OID (char d0,char d1,char d2,char d3){
		data[0]=(unsigned char)d0;	
		data[1]=(unsigned char)d1;
		data[2]=(unsigned char)d2;
		data[3]=(unsigned char)d3;
	}	

	unsigned char& operator[](int i){
		return data[i];	
	}

	OID& operator=(OID& o){
		for(int i=0;i<4;i++)this->data[i]=o.data[i];
		return *this;
	}

	bool operator<(OID& o){
		for(int i=0;i<4;i++)
			if(data[i]<o.data[i])return true;
			else if(o.data[i]<data[i])return false;
		return false;
	}

	OID& operator++(){
		for(int i=3;i>=0;i--){
			data[i]++;
			if(data[i]!=0)return *this;
		}
		return *this;
	}

	OID& operator|(OID& o){		
		for(int i=0;i<4;i++){
			data[i]=data[i]|o.data[i];
		}
		return *this;
	}

	OID& operator&(OID& o){
		
		for(int i=0;i<4;i++){
			data[i]=data[i]&o.data[i];
		}
		return *this;
	}


	bool operator==(OID& o){
		for(int i=0;i<4;i++)
			if(o.data[i]!=data[i])return false;
		return true;	
	}


	bool operator!=(OID& o){
		return !(*this==o);	
	}

	char* toString(){
		char*result= new char[17];//<a1a2a3_a4a5a6_a7a8a9_a10a11a12_'\0'>=17 chars	
		int resultCursor=0;
		int curData;
		int curDigit;
		for(int i=0;i<4;i++) {
			//Read the number
			curData=((unsigned int)data[i]);
	
			//Extract first digit
			curDigit= (curData-(curData%100))/100;		
			//Save first digit:
			result[4*i]=(char)(curDigit + '0');
			//Actualize curData
			curData-=curDigit*100;

			//Extract second digit:
			curDigit=(curData-curData%10)/10;
			//Save second digit
			result[4*i+1]=(char)(curDigit+'0');
			//Actualize curData
			curData-=curDigit*10;

			//Extract third digit
			curDigit=curData;
			//Save third digit
			result[4*i+2]=(char)(curDigit+'0');

			//Save fourth digit (underscore)
			result[4*i+3]='_';
		
		}
		result[16]='\0';
		return result;
	}

	char * toDotString(){
		char*result=new char[16];
		int index=0;
		for(int curIndex=0,curNumber;curIndex<4;curIndex++){
			bool skippingZeros=true;
			//escribimos this->data[curIndex];
			curNumber=data[curIndex];
			int digit;
			if(curNumber>=100){
				digit= (int)((curNumber-(curNumber%100))/	100);	
				result[index]=digit+'0';
				index++;
				curNumber-=(digit*100);
				skippingZeros=false;
			}


			if(!skippingZeros||curNumber>=10){
				digit= (int)((curNumber-(curNumber%10))/	10);	
				result[index]=digit+'0';
				index++;
				curNumber-=(digit*10);
			}
			result[index]=curNumber+'0';
			index++;
			result[index]='.';
			index++;
		}	
		index--;
		result[index]='\0';
		return result;
	}
	/*char* toDotString(){
		char*result= new char[16];//<a1a2a3.a4a5a6.a7a8a9.a10a11a12'\0'>=16 chars	
		int resultCursor=0;
		int curData;
		int curDigit;
		for(int i=0;i<4;i++) {
			//Read the number
			curData=((unsigned int)data[i]);
	
			//Extract first digit
			curDigit= (curData-(curData%100))/100;		
			//Save first digit:
			result[4*i]=(char)(curDigit + '0');
			//Actualize curData
			curData-=curDigit*100;

			//Extract second digit:
			curDigit=(curData-curData%10)/10;
			//Save second digit
			result[4*i+1]=(char)(curDigit+'0');
			//Actualize curData
			curData-=curDigit*10;

			//Extract third digit
			curDigit=curData;
			//Save third digit
			result[4*i+2]=(char)(curDigit+'0');

			//Save fourth digit (underscore)
			result[4*i+3]='.';
		
		}
		result[15]='\0';
		return result;
	}*/
};

static const OID INVALID_OID(0,0,0,0);
static const OID ROOT_OID(0,0,0,1);

#endif

