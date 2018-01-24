// as of right now all lines were copied from the original pascal file

{ This unit contains functions useful for dealing with filenames. }

{$G+}{$N+}{$E+}{$X+}{$D-}{$S-}{$V-}
Unit fileLib;

INTERFACE

Uses
  Dos;

//these are defining functions that were not defined in ATR2FUNC, addfront and addrear were defined there
  Function EXIST(thisfile : pathstr) : boolean;
  Function VALID(thisfile : pathstr) : boolean;
  Function name_form(name:string):string;
  function exten(name:string):string;
  function base_name(name:string):string;
  Function attribs(b:byte):string;
  function path(fn:string):string;
  function no_path(fn:string):string;
  function file_length(fn:string):longint;

IMPLEMENTATION

Const
  null  = #0;
  bell  = #7;
  esc   = #27;
  f10   = #$44; {scan code}
  basex : byte = 1;
  basey : byte = 1;
  tempx : byte = 1;
  tempy : byte = 1;
  endx  : byte = 24;
  endy  : byte = 80;

Var
  regs        : registers;
  textattr    : word;
  workstr     : string;


Function addfront(b:string;l:integer): string; //function that takes in a string (b) and an integer (L)
Begin
  while length(b)< l do //length of b < L
    b := ' '+b; //if b = hi, then it adds a space in front of whatever is b, (i.e.: " hi" for 1 loop
  addfront := b; //returns b
End;

Function addrear(b:string;l:integer): string; //function that takes in a string (b) and an integer (L)
Begin
  while length(b)< l do //length of b < L
    b := b+' '; //if b = hi, then it adds a space behind whatever b is, (i.e.: "hi " for 1 loop
  addrear := b; //returns b
End;

//both addfront and addrear do not seem necessary, after tracing and determining where the functions lie in every file, it seems their only use is indentation which can be done in c

function lstr(s1:string; l:integer):string; //function that takes in a string (s1) and an integer (L)
begin
 if length(s1)<=l then lstr:=s1 //checks if the length of s1 is greater than or equal to L, if it is then it returns s1
 else lstr:=copy(s1,1,l); //copy is a pascal function in the library, link to its use: https://www.freepascal.org/docs-html/rtl/system/copy.html
//Copy returns a string which is a copy if the Count characters in S, starting at position Index. If Count is larger than the length of the string S, the result is truncated. If Index is larger than the length of the string S, then an empty string is returned. Index is 1-based.
end;

//this function is almost an exact copy of the one above it
function rstr(s1:string; l:integer):string; //function that takes in a string (s1) and integer (length)
begin
 if length(s1)<=l then rstr:=s1 //checks if the length is greater than or equal to s1, if it is then it returns s1
 else rstr:=copy(s1,length(s1)-l+1,l); //sends the copy function (see above) string s1, the length of s1 - 1 + variable l, and l
end;

//probably a vital function -- may be able to convert to a better function though
Function EXIST(thisfile : pathstr) : boolean; //function takes in a file (thisfile) as a path
  var
    afile : file; //afile is an empty file
    iocode : word; //word is an unsigned 16-bits integer https://www.freepascal.org/docs-html/rtl/system/word.html

  begin {* fExist *}
    assign(afile,thisfile); //afile = thisfile (the parameter file is put in the empty file)
    {$I-} //the compiler is generating I/O checking code, can be replaced by throwing exceptions
    reset(afile); //https://www.freepascal.org/docs-html/rtl/system/reset.html
//Reset opens a file F for reading. F can be any file type. If F is a text file, or refers to standard I/O (e.g : '') then it is opened read-only, otherwise it is opened using the mode specified in filemode. If F is an untyped file, the record size can be specified in the optional parameter L. A default value of 128 is used. File sharing is not taken into account when calling Reset.
    iocode := ioresult; //IOresult contains the result of any input/output call, when the {\$i-} compiler directive is active, disabling IO checking. When the flag is read, it is reset to zero. If IOresult is zero, the operation completed successfully. If non-zero, an error occurred.
//iocode will receive whatever is returned from ioresult
    {$I+} //same as previous https://www.freepascal.org/docs-html/prog/progsu38.html
    Exist := (iocode = 0); //EXIST returns boolean
    if iocode = 0 then close(afile); //closes file
  end;  {* fExist *}

//this function seems important, as it calls Exist (above), however it seems to have a lot of dead weight. for example, boolean check is never used. As well, once the file is opened, nothing is done with it. It seems that this function is just used to check to see if the file can open
Function VALID(thisfile : pathstr) : boolean; //function takes in a file thisfile as a path
  Var
    afile : file; //afile is an empty file
    check : boolean;
    iocode : word; //word is an unsigned 16 bit int (see above)

  begin {* fValid *}
    if not Exist(thisfile) then //checks if the file exists, via the previous function
      begin
        assign(afile,thisfile); //afile = thisfile
        {$I-} //turns off error checking in I/O
        rewrite(afile); //sends file to function rewrite, which opens the file for writing https://www.freepascal.org/docs-html/rtl/system/rewrite.html
        close(afile); //closes afile
        erase(afile); //deletes afile
        {$I+} //turns error checking in I/O back on
        iocode := ioresult; //ioresult basically checks if there were any errors with I/O while $I was off https://www.freepascal.org/docs-html/rtl/system/ioresult.html
        Valid := (iocode = 0) //returns ioresult's error code, or 0 for error free
      end
    else Valid := true //otherwise, returns true
  end;  {* fValid *}

  //may be useful, but can be much better than how it is written now
Function name_form(name:string):string; //function taking in a string (name) 
var
 i,j,k,l:integer;
 s1,s2,s3:string;
begin
 s1:=''; s2:=''; //sets variables to nothing
 k:=1; //sets only one variable equal to 1
 if (name='.') or (name='..') then //checks name
  begin
   name_form:=addrear(name,12); //takes the name to addrear (can probably just throw an exception)
   exit; 
  end;
 while (k<=length(name)) and (name[k]<>'.') do //goes through each character of name until it hits .
  begin
   s1:=s1+name[k]; //moves the characters to s1
   inc(k); //k++
  end;
 if k<length(name) then //I believe this one is to get what is after the . but that can all be done in one loop
  begin
   inc(k); //k++
   while (k<=length(name)) and (name[k]<>'.') do 
    begin
     s2:=s2+name[k]; //moves characters to s2
     inc(k);
    end;
  end;
 name_form:=addrear(s1,9)+addrear(s2,3); //returns the name of the file but formatted
end;

//seems similar to the functions above and below it, lots of redundency and such, and the function does nothing with s1 after working with it. My best guess is that these three functions are different versions of completing the same job, as if rewriting without deleting 
function exten(name:string):string; //function passed string name
var
 i,j,k,l:integer;
 s1,s2,s3:string;
begin
 s1:=''; s2:=''; //set s1 and s2 as blank
 k:=1; //set k to 1
 while (k<=length(name)) and (name[k]<>'.') do //sets s1 to name, minus any .
  begin
   s1:=s1+name[k];
   inc(k);
  end;
 if k<length(name) then //if the previous loop stopped early, due to a period or such,
  begin
   inc(k); //skip the period
   while (k<=length(name)) and (name[k]<>'.') do //and do it again sith s2
    begin
     s2:=s2+name[k];
     inc(k);
    end;
  end;
 exten:=addrear(s2,3); //passes addrear() s2 and the value 3, then returns the return
end;

//definitely not necessary because that was already done in name_form, both functions can be much better
function base_name(name:string):string; //function that takes in a string (name)
var
 i,j,k,l:integer;
 s1,s2,s3:string;
begin
 s1:=''; s2:=''; 
 k:=1; 
 while (k<=length(name)) and (name[k]<>'.') do //basically a for loop
  begin
   s1:=s1+name[k]; //this is just getting the beginning portion of a file name 
   inc(k); 
  end;
 base_name:=s1; //returns file name
end;

//this function determines the type of file from a byte being passed to it. it has a lot of redundancy, and can probably be done better
Function attribs(b:byte):string; //function passed a byte
{const
   ReadOnly = $01;
   Hidden   = $02;
   SysFile  = $04;
   VolumeID = $08;
   Directory= $10;
   Archive  = $20;
   AnyFile  = $3F;} //the hex codes for different file types, note the comments
var
s1:string[8];
begin
 s1:=' '; //assuming each file is only one type, this can easily be done with a switch
 if (b and readonly)<>0  then s1:=s1+'R' else s1:=s1+'.'; 
 if (b and hidden  )<>0  then s1:=s1+'H' else s1:=s1+'.';
 if (b and sysfile )<>0  then s1:=s1+'S' else s1:=s1+'.';
 if (b and archive )<>0  then s1:=s1+'A' else s1:=s1+'.'; //this section adds an identifier to a string for the different filetypes. note how not all are used
 attribs:=s1; //returns string s1, containing the identifiers
end;

//vital depending on how we recieve the file
function path(fn:string):string; //function takes in string (fn)
var
 i,k:integer;
begin
 k:=0;
 for i:=length(fn) downto 1 do //just a for loop, downto is used if the initial value is less than the final value, i starts with the length of the brought in string https://www.freepascal.org/docs-html/ref/refsu58.html
  begin
   if ((fn[i]='\') or (fn[i]=':')) and (k<i) then k:=i; //checks for end of path indicators, then has k = i
  end;
 if k<>0 then
   path:=lstr(fn,k) //sends what is found to the lstr function to be returned
  else
   path:=''; //returns nothing if at end of path
end;

//is almost the exact same, line for line, as the above function. this matches how functions lstr() and rstr() are also almost the same. the added redundancy seems redundant.
function no_path(fn:string):string; //function passed string fn
var
 i,k:integer;
begin
 k:=0; //set k = 0
 for i:=length(fn) downto 1 do //for loop
  begin
   if ((fn[i]='\') or (fn[i]=':')) and (k<i) then k:=i; //sets k to equal the last \ or : in fn
  end;
 if k<>0 then
   no_path:=rstr(fn,length(fn)-k) //if there are '\'s or ':'s, pass function rstr string fn, and the length of fn -k. returns the result
  else
   no_path:=fn; //otherwise, returns the original, untouch string
end;

//definitely essential, easy to convert into c though, and would be better off using pointers
function file_length(fn:string):longint;
var
 sr:searchrec;
begin
 findfirst(fn,archive,sr);
 if doserror=0 then file_length:=sr.size
               else file_length:=0;
end;

end. Unit
