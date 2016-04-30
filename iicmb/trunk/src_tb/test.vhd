
--==============================================================================
--                                                                             |
--    Project: IIC Multiple Bus Controller (IICMB)                             |
--                                                                             |
--    Module:  Test package.                                                   |
--    Version:                                                                 |
--             1.0,   April 29, 2016                                           |
--                                                                             |
--    Author:  Sergey Shuvalkin, (sshuv2@opencores.org)                        |
--                                                                             |
--==============================================================================
--==============================================================================
-- Copyright (c) 2016, Sergey Shuvalkin                                        |
-- All rights reserved.                                                        |
--                                                                             |
-- Redistribution and use in source and binary forms, with or without          |
-- modification, are permitted provided that the following conditions are met: |
--                                                                             |
-- 1. Redistributions of source code must retain the above copyright notice,   |
--    this list of conditions and the following disclaimer.                    |
-- 2. Redistributions in binary form must reproduce the above copyright        |
--    notice, this list of conditions and the following disclaimer in the      |
--    documentation and/or other materials provided with the distribution.     |
--                                                                             |
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" |
-- AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   |
-- IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  |
-- ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE    |
-- LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         |
-- CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        |
-- SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    |
-- INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     |
-- CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     |
-- ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  |
-- POSSIBILITY OF SUCH DAMAGE.                                                 |
--==============================================================================


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


--==============================================================================
package test is

  type integer_array     is array (natural range <>) of integer;
  type integer_array_ptr is access integer_array;
  type string_ptr        is access string;

  type char_file         is file of character;
  ------------------------------------------------------------------------------
  -- The file type declaration above implicitly defines the following operations:
  -- procedure file_open (                               file f : char_file; name : in string; open_kind : in file_open_kind := read_mode);
  -- procedure file_open (status : out file_open_status; file f : char_file; name : in string; open_kind : in file_open_kind := read_mode);
  -- procedure file_close(file f : char_file);
  -- procedure read (file f : char_file; value : out character);
  -- procedure write(file f : char_file; value : in  character);
  -- function  endfile(file f : char_file) return boolean;
  ------------------------------------------------------------------------------

  file stdin  : char_file open read_mode  is "STD_INPUT";
  file stdout : char_file open write_mode is "STD_OUTPUT";

  ------------------------------------------------------------------------------
  -- Printing to a file and stdout:
  constant newline : string := "" & lf;

  procedure fprint_string(file f : char_file; value : in string);
  procedure print_string(value : in string);
  --function to_string(value : in integer; width : in positive := 1) return string;
  --function to_string(value : in std_logic_vector) return string;

  ------------------------------------------------------------------------------
  -- Function for converting integer to string:
  function to_string(value  : in integer;
                     format : in string  := "d";
                     width  : in natural := 0;
                     prec   : in natural := 0) return string;
  function to_string(value  : in std_logic;
                     format : in string  := "b";
                     width  : in natural := 0;
                     prec   : in natural := 0) return string;
  function to_string(value  : in std_logic_vector;
                     format : in string  := "x";
                     width  : in natural := 0;
                     prec   : in natural := 0) return string;
  --
  -- format string: [FLAGS][WIDTH][.PREC]TYPE
  --
  --     * FLAGS
  --        an optional sequence of characters which control output
  --        justification, numeric signs, trailing zeroes, and octal and hex
  --        prefixes.  The flag characters are minus (`-'), plus (`+'), space
  --        ( ), zero (`0'), and sharp (`#').  They can appear in any
  --        combination.
  --
  --       `-'   The result of the conversion is left justified, and the right
  --             is padded with blanks.  If you do not use this flag, the
  --             result is right justified, and padded on the left.
  --
  --       `+'   The result of a signed conversion (as determined by TYPE)
  --             will always begin with a plus or minus sign.  (If you do not
  --             use this flag, positive values do not begin with a plus sign.)
  --
  --       `" " (space)'
  --             If the first character of a signed conversion specification
  --             is not a sign, or if a signed conversion results in no
  --             characters, the result will begin with a space.  If the space
  --             ( ) flag and the plus (`+') flag both appear, the space flag
  --             is ignored.
  --
  --       `0'   Leading zeroes are used to pad the field width (following any
  --             indication of sign or base); no spaces are used for padding.
  --             If the zero (`0') and minus (`-') flags both appear, the zero
  --             (`0') flag will be ignored.  If a precision PREC is specified,
  --             the zero (`0') flag is ignored.  Note that `0' is interpreted
  --             as a flag, not as the beginning of a field width.
  --   
  --       `#'   The result is to be converted to an alternative form,
  --             depending on the conversion type character:
  --
  --            `b'   result will have a `b' prefix.
  --
  --            `o'   increases precision to force the first digit of the
  --                  result to be a zero.
  --
  --            `x'   result will have a `0x' prefix.
  --
  --            `X'   result will have a `0X' prefix.
  --
  --            `all others'
  --                  ignored.
  --
  --     * WIDTH
  --        WIDTH is an optional minimum field width.  You can either specify
  --        it directly as a decimal integer, or indirectly by using instead
  --        an asterisk (`*'), in which case the 'width' argument is used as
  --        the field width.
  --
  --     * PREC
  --        an optional field; if present, it is introduced with "." (a
  --        period).  This field gives the minimum number of digits of an
  --        integer to print.  You can specify the precision either directly as
  --        a decimal integer or indirectly by using an asterisk (`*'), in which
  --        case a 'prec' argument is used as the precision.  If only a period
  --        is specified the precision is zero.
  --
  --     * TYPE
  --        TYPE field specifies what kind of conversion to_string() performs.
  --        Here is a table of these:
  --
  --       `b'   prints a unsigned binary integer;
  --
  --       `d'   prints a signed decimal integer; (same as `i')
  --
  --       `i'   prints a signed decimal integer; (same as `d')
  --
  --       `o'   prints an unsigned octal integer;
  --
  --       `u'   prints an unsigned decimal integer;
  --
  --       `x'   prints an unsigned hexadecimal integer (using `abcdef' as
  --             digits beyond `9');
  --
  --       `X'   prints an unsigned hexadecimal integer (using `ABCDEF' as
  --             digits beyond `9');
  ------------------------------------------------------------------------------


  ------------------------------------------------------------------------------
  -- format: [FLAGS][WIDTH][.PREC][SIZE][TYPE]
  --
  --     * FLAGS
  --
  --        an optional sequence of characters which control output
  --        justification, numeric signs, decimal points, trailing zeroes, and
  --        octal and hex prefixes.  The flag characters are minus (`-'), plus
  --        (`+'), space ( ), zero (`0'), and sharp (`#').  They can appear in
  --        any combination.
  --
  --       `-'
  --             The result of the conversion is left justified, and the right
  --             is padded with blanks.  If you do not use this flag, the
  --             result is right justified, and padded on the left.
  --
  --       `+'
  --             The result of a signed conversion (as determined by TYPE)
  --             will always begin with a plus or minus sign.  (If you do not
  --             use this flag, positive values do not begin with a plus sign.)
  --
  --       `" " (space)'
  --             If the first character of a signed conversion specification
  --             is not a sign, or if a signed conversion results in no
  --             characters, the result will begin with a space.  If the space
  --             ( ) flag and the plus (`+') flag both appear, the space flag
  --             is ignored.
  --
  --       `0'
  --             If the TYPE character is `d', `i', `o', `u', `x', `X', `e',
  --             `E', `f', `g', or `G': leading zeroes, are used to pad the
  --             field width (following any indication of sign or base); no
  --             spaces are used for padding.  If the zero (`0') and minus
  --             (`-') flags both appear, the zero (`0') flag will be ignored.
  --             For `d', `i', `o', `u', `x', and `X' conversions, if a
  --             precision PREC is specified, the zero (`0') flag is ignored.
  --             Note that `0' is interpreted as a flag, not as the beginning
  --             of a field width.
  --   
  --       `#'
  --             The result is to be converted to an alternative form,
  --             according to the next character:
  --
  --            `0'
  --                  increases precision to force the first digit of the
  --                  result to be a zero.
  --
  --            `x'
  --                  a non-zero result will have a `0x' prefix.
  --
  --            `X'
  --                  a non-zero result will have a `0X' prefix.
  --
  --            `e, E or f'
  --                  The result will always contain a decimal point even if
  --                  no digits follow the point.  (Normally, a decimal point
  --                  appears only if a digit follows it.)  Trailing zeroes
  --                  are removed.
  --
  --            `g or G'
  --                  same as `e' or `E', but trailing zeroes are not removed.
  --
  --            `all others'
  --                  undefined.
  --
  --      * WIDTH
  --
  --        WIDTH is an optional minimum field width.  You can either specify
  --        it directly as a decimal integer, or indirectly by using instead
  --        an asterisk (`*'), in which case an `int' argument is used as the
  --        field width.  Negative field widths are not supported; if you
  --        attempt to specify a negative field width, it is interpreted as a
  --        minus (`-') flag followed by a positive field width.
  --
  --      * PREC
  --
  --        an optional field; if present, it is introduced with ``.'' (a
  --        period). This field gives the maximum number of characters to
  --        print in a conversion; the minimum number of digits of an integer
  --        to print, for conversions with TYPE `d', `i', `o', `u', `x', and
  --        `X'; the maximum number of significant digits, for the `g' and `G'
  --        conversions; or the number of digits to print after the decimal
  --        point, for `e', `E', and `f' conversions.  You can specify the
  --        precision either directly as a decimal integer or indirectly by
  --        using an asterisk (`*'), in which case an `int' argument is used
  --        as the precision.  Supplying a negative precision is equivalent to
  --        omitting the precision.  If only a period is specified the
  --        precision is zero.  If a precision appears with any other
  --        conversion TYPE than those listed here, the behavior is undefined.
  --
  --      * SIZE
  --
  --        `h', `l', and `L' are optional size characters which override the
  --        default way that `printf' interprets the data type of the
  --        corresponding argument.  `h' forces the following `d', `i', `o',
  --        `u', `x' or `X' conversion TYPE to apply to a `short' or `unsigned
  --        short'. `h' also forces a following `n' TYPE to apply to a pointer
  --        to a `short'. Similarily, an `l' forces the following `d', `i',
  --        `o', `u', `x' or `X' conversion TYPE to apply to a `long' or
  --        `unsigned long'.  `l' also forces a following `n' TYPE to apply to
  --        a pointer to a `long'.  `l' with `c', `s' is equivalent to `C',
  --        `S' respectively.  If an `h' or an `l' appears with another
  --        conversion specifier, the behavior is undefined.  `L' forces a
  --        following `e', `E', `f', `g' or `G' conversion TYPE to apply to a
  --        `long double' argument.  If `L' appears with any other conversion
  --        TYPE, the behavior is undefined.
  --
  --      * TYPE
  --
  --        TYPE specifies what kind of conversion `printf' performs.  Here is
  --        a table of these:
  --
  --       `%'
  --             prints the percent character (`%')
  --
  --       `c'
  --             prints ARG as single character
  --
  --       `C'
  --             prints wchar_t ARG as single multibyte character
  --
  --       `s'
  --             prints characters until precision is reached or a null
  --             terminator is encountered; takes a string pointer
  --
  --       `S'
  --             converts wchar_t characters to multibyte output characters
  --             until precision is reached or a null wchar_t terminator is
  --             encountered; takes a wchar_t pointer
  --
  --       `d'
  --             prints a signed decimal integer; takes an `int' (same as `i')
  --
  --       `i'
  --             prints a signed decimal integer; takes an `int' (same as `d')
  --
  --       `o'
  --             prints a signed octal integer; takes an `int'
  --
  --       `u'
  --             prints an unsigned decimal integer; takes an `int'
  --
  --       `x'
  --             prints an unsigned hexadecimal integer (using `abcdef' as
  --             digits beyond `9'); takes an `int'
  --
  --       `X'
  --             prints an unsigned hexadecimal integer (using `ABCDEF' as
  --             digits beyond `9'); takes an `int'
  --
  --       `f'
  --             prints a signed value of the form `[-]9999.9999'; takes a
  --             floating-point number
  --
  --       `e'
  --             prints a signed     value of the form
  --             `[-]9.9999e[+|-]999'; takes a floating-point number
  --
  --       `E'
  --             prints the same way as `e', but using `E' to introduce the
  --             exponent; takes a floating-point number
  --
  --       `g'
  --             prints a signed value in either `f' or `e' form, based on
  --             given value and precision--trailing zeros and the decimal
  --             point are printed only if necessary; takes a floating-point
  --             number
  --
  --       `G'
  --             prints the same way as `g', but using `E' for the exponent if
  --             an exponent is needed; takes a floating-point number
  --
  --       `n'
  --             stores (in the same object) a count of the characters written;
  --             takes a pointer to `int'
  --
  --       `p'
  --             prints a pointer in an implementation-defined format.  This
  --             implementation treats the pointer as an `unsigned long' (same
  --             as `Lu').
  ------------------------------------------------------------------------------

  -- Conversion functions
  function to_integer         (value : in string) return integer;




  procedure read_identifier(file f : char_file; value : inout string_ptr);
  procedure read_integer(file f : char_file; value : out integer);
  procedure find_token(file f : char_file; value : in string);
  ------------------------------------------------------------------------------


  ------------------------------------------------------------------------------
  -- Working with commands:
  ------------------------------------------------------------------------------
  -- First level of abstraction: tokens.
  constant max_token_size : integer := 80;
  type token_kind is
  (
    name,
    integer_number,
    end_of_line,
    end_of_file,
    start_of_comment,
    separator,
    not_recognized
  );

  type token is record
    kind      : token_kind;
    value_ptr : string_ptr;
  end record;

  procedure read_token(file f : char_file; value : inout token);
  procedure write_token(file f : char_file; value : inout token);
  --

  ------------------------------------------------------------------------------
  -- Second level of abstraction: commands.
  -- Maximum number of arguments in a command:
  constant max_args_in_command : integer := 4*1024;

  -- Command type:
  type command is record
    time_cnt : natural;               -- The time, when the command should be applied
    id       : string_ptr;            -- Pointer to the command ID.
    arg      : integer_array_ptr; -- Pointer to the array of arguments.
  end record;

  procedure read_command(file f : char_file; id : in string; cmd : inout command);

  type command_list_elem;
  type command_list_elem_ptr is access command_list_elem;
  type command_list_elem is record
    cmd      : command;
    prv      : command_list_elem_ptr;
    nxt      : command_list_elem_ptr;
  end record;
  type command_list is record
    number   : natural;
    first    : command_list_elem_ptr;
    last     : command_list_elem_ptr;
  end record;
  procedure init_command_list(fname : in string; variable clist : inout command_list; block_id : in string);
  procedure put_command(variable clist : inout command_list; cmd : inout command);
  procedure get_command(variable clist : inout command_list; cmd : inout command);
  ------------------------------------------------------------------------------

end test;
--==============================================================================

--==============================================================================
package body test is

  constant hexadecimal_chars : string(1 to 32) := "0123456789abcdef0123456789ABCDEF";

  ------------------------------------------------------------------------------
  function to_character(value : in std_logic) return character is
  begin
    case value is
      when 'U' => return 'U';
      when 'X' => return 'X';
      when '0' => return '0';
      when '1' => return '1';
      when 'Z' => return 'Z';
      when 'W' => return 'W';
      when 'L' => return 'L';
      when 'H' => return 'H';
      when '-' => return '-';
    end case;
  end function to_character;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  procedure fprint_string (file f : char_file; value : in string) is
  begin
    for i in value'low to value'high loop
      write(f, value(i));
    end loop;
  end procedure fprint_string;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  procedure print_string(value : in string) is
  begin
    fprint_string(stdout, value);
  end procedure print_string;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  procedure read_integer(file f : char_file; value : out integer) is
    variable buf  : string(1 to 256);
    variable ch0  : character;
    variable idx  : positive;
  begin
    -- Searching for first character:
    while true loop
      assert not(endfile(f)) report "read_integer(): Unexpected End Of File." severity failure;
      read(f, ch0);
      exit when ((ch0 >= '0')and(ch0 <= '9'))or(ch0 = '-');
    end loop;

    buf(1) := ch0;
    idx    := 2;

    -- Reading other characters:
    while (not(endfile(f))) loop
      read(f, ch0);
      if ((ch0 >= '0')and(ch0 <= '9'))or((ch0 >= 'a')and(ch0 <= 'f'))or((ch0 >= 'A')and(ch0 <= 'F'))or(ch0 = 'x') then
        buf(idx) := ch0;
        idx := idx + 1;
      else
        exit;
      end if;
    end loop;
    value := to_integer(buf(1 to idx - 1));
  end procedure read_integer;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  procedure read_identifier(file f : char_file; value : inout string_ptr) is
    variable buf  : string(1 to 256);
    variable ch0  : character;
    variable idx  : positive;
  begin
    -- Searching for first character:
    while true loop
      assert not(endfile(f)) report "read_identifier(): Unexpected End Of File." severity failure;
      read(f, ch0);
      exit when ((ch0 >= 'A')and(ch0 <= 'Z'))or((ch0 >= 'a')and(ch0 <= 'z'))or(ch0 = '_');
    end loop;

    buf(1) := ch0;
    idx    := 2;

    -- Reading other characters:
    while (not(endfile(f))) loop
      read(f, ch0);
      if ((ch0 >= 'A')and(ch0 <= 'Z'))or((ch0 >= 'a')and(ch0 <= 'z'))or((ch0 >= '0')and(ch0 <= '9'))or(ch0 = '_') then
        buf(idx) := ch0;
        idx := idx + 1;
      else
        exit;
      end if;
    end loop;
    deallocate(value);
    value := new string'(buf(1 to idx - 1));
  end procedure read_identifier;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  procedure find_token(file f : char_file; value : in string) is
    variable ch0  : character;
    variable idx  : positive;
  begin
    idx := value'low;
    while true loop
      assert not(endfile(f)) report "find_token(" & value & "): Unexpected End Of File." severity failure;
      read(f, ch0);
      if (ch0 = value(idx)) then
        exit when (idx = value'high);
        idx := idx + 1;
      else
        idx := value'low;
      end if;
    end loop;
  end procedure find_token;
  ------------------------------------------------------------------------------


  ------------------------------------------------------------------------------
  function to_integer(value : in string) return integer is
    variable value_local : integer := 0;
    variable start_idx   : positive;
    variable negative    : boolean;
  begin
    if (value'right > 2)and(value(1) = '0')and(value(2) = 'x') then -- hex format
      start_idx := 3;
        
      for i in start_idx to value'right loop
        if    (value(i) >= 'a')and(value(i) <= 'f') then
          value_local := (16 * value_local) + (character'pos(value(i)) - character'pos('a') + 10);
        elsif (value(i) >= 'A')and(value(i) <= 'F') then
          value_local := (16 * value_local) + (character'pos(value(i)) - character'pos('A') + 10);
        else
          value_local := (16 * value_local) + (character'pos(value(i)) - character'pos('0'));
        end if;
      end loop;
    else
      if (value(1) = '-') then
        negative  := true;
        start_idx := 2;
      else
        negative  := false;
        start_idx := 1;
      end if;
        
      for i in start_idx to value'right loop
        value_local := (10 * value_local) + (character'pos(value(i)) - character'pos('0'));
      end loop;

      if (negative) then
        value_local := (-1) * value_local;
      end if;
    end if;

    return value_local;
  end function to_integer;
  ------------------------------------------------------------------------------

  --function to_string(value : in integer; width : in positive := 1) return string is
  --  variable idx  : positive;
  --  variable buf  : string(1 to 80);
  --  variable negative : boolean := (value < 0);
  --  variable value_local : integer := abs(value);
  --begin
  --  idx := 80;

  --  if (value_local = 0) then
  --    buf(idx) := '0';
  --    idx := idx - 1;
  --  end if;

  --  while (value_local /= 0) loop
  --    buf(idx) := character'val((value_local rem 10) + character'pos('0'));
  --    value_local := value_local / 10;
  --    idx := idx - 1;
  --  end loop;

  --  if (negative) then
  --    buf(idx) := '-';
  --    idx := idx - 1;
  --  end if;

  --  while ((80 - idx) < width) loop
  --    buf(idx) := ' ';
  --    idx := idx - 1;
  --  end loop;
    
  --  buf(1 to (80 - idx)) := buf((1 + idx) to 80);

  --  return buf(1 to (80 - idx));
  --end function to_string;

  --function to_string(value : in std_logic_vector) return string is
  --  variable res_string : string((((value'length + 3)/4) + 2) downto 1);
  --  variable res_idx    : positive;
  --  variable hex_digit  : integer;
  --begin

  --  hex_digit := 0;
  --  res_idx   := 1;
  --  for idx in value'right to value'left loop
  --    if    (value(idx)  = '1') then
  --      hex_digit := hex_digit + 2**((idx - value'right) rem 4);
  --    elsif (value(idx) /= '0') then
  --      hex_digit := hex_digit + 100;
  --    end if;
  --    if (((idx - value'right) rem 4) = 3)or(idx = value'left) then
  --      case hex_digit is
  --        when  0     => res_string(res_idx) := '0';
  --        when  1     => res_string(res_idx) := '1';
  --        when  2     => res_string(res_idx) := '2';
  --        when  3     => res_string(res_idx) := '3';
  --        when  4     => res_string(res_idx) := '4';
  --        when  5     => res_string(res_idx) := '5';
  --        when  6     => res_string(res_idx) := '6';
  --        when  7     => res_string(res_idx) := '7';
  --        when  8     => res_string(res_idx) := '8';
  --        when  9     => res_string(res_idx) := '9';
  --        when 10     => res_string(res_idx) := 'A';
  --        when 11     => res_string(res_idx) := 'B';
  --        when 12     => res_string(res_idx) := 'C';
  --        when 13     => res_string(res_idx) := 'D';
  --        when 14     => res_string(res_idx) := 'E';
  --        when 15     => res_string(res_idx) := 'F';
  --        when others => res_string(res_idx) := 'X';
  --      end case;
  --      hex_digit := 0;
  --      res_idx   := res_idx + 1;
  --    end if;
  --  end loop;
  --  res_string(res_string'left)     := '0';
  --  res_string(res_string'left - 1) := 'x';

  --  return res_string;
  --end function to_string;


  ------------------------------------------------------------------------------
  -- Working with commands:
  ------------------------------------------------------------------------------
  -- First level of abstraction: tokens.
  procedure read_token(file f : char_file; value : inout token) is
    variable ch0  : character;
    variable idx  : integer := 1;
    variable end_of_token : boolean := false;
    variable buf  : string(1 to max_token_size);
    variable kind : token_kind;
  begin
    deallocate(value.value_ptr);
    while (idx <= max_token_size)and(end_of_token = false) loop
      if (idx = 1) then
        if endfile(f) then
          kind         := end_of_file;
          end_of_token := true;
        else
          read(f, ch0);
          next when (ch0 = ' ')or(ch0 = ht);
          buf(idx) := ch0;
          idx := idx + 1;

          if    (ch0 = '#') then
            kind         := start_of_comment;
            end_of_token := true;
          elsif ((ch0 >= '0')and(ch0 <= '9'))or(ch0 = '-') then
            kind         := integer_number;
          elsif ((ch0 >= 'A')and(ch0 <= 'Z'))or((ch0 >= 'a')and(ch0 <= 'z'))or(ch0 = '_') then
            kind         := name;
          elsif (ch0 = cr) then
            kind         := end_of_line;
          elsif (ch0 = lf) then
            kind         := end_of_line;
            end_of_token := true;
          elsif (ch0 = ',')or(ch0 = '(')or(ch0 = ')')or(ch0 = '{')or(ch0 = '}')or(ch0 = ':') then
            kind         := separator;
            end_of_token := true;
          else
            kind         := not_recognized;
            end_of_token := true;
          end if;
        end if;
      else
        if endfile(f) then
          end_of_token := true;
        else
          read(f, ch0);
          case kind is
            when integer_number =>
              if ((ch0 >= '0')and(ch0 <= '9'))or((ch0 >= 'a')and(ch0 <= 'f'))or((ch0 >= 'A')and(ch0 <= 'F'))or(ch0 = 'x') then
                buf(idx) := ch0;
                idx := idx + 1;
              else
                end_of_token := true;
              end if;
            when name           =>
              if ((ch0 >= 'A')and(ch0 <= 'Z'))or((ch0 >= 'a')and(ch0 <= 'z'))or((ch0 >= '0')and(ch0 <= '9'))or(ch0 = '_') then
                buf(idx) := ch0;
                idx := idx + 1;
              else
                end_of_token := true;
              end if;
            when end_of_line    =>
              if (ch0 = lf) then
                buf(idx) := ch0;
                idx := idx + 1;
              end if;
              end_of_token := true;
            when others         =>
              end_of_token := true;
          end case;
        end if;
      end if;
    end loop;

    value.kind      := kind;
    value.value_ptr := new string'(buf(1 to idx - 1));
  end procedure read_token;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  procedure write_token(file f : char_file; value : inout token) is
  begin
    fprint_string(f, value.value_ptr.all & ' ');
  end procedure write_token;
  ------------------------------------------------------------------------------
  --

  ------------------------------------------------------------------------------
  -- Second level of abstraction: commands.
  -- <integer_number> <name0> <name1> <{> <integer_number> ... <}>
  procedure read_command(file f : char_file; id : in string; cmd : inout command) is
    variable tmp_token : token;
    variable token_idx : integer := 1;
    variable arg_idx   : integer := 1;
    variable tmp_array : integer_array(1 to max_args_in_command);
  begin
    while true loop
      read_token(f, tmp_token);

      case tmp_token.kind is
        when name             =>
          assert (token_idx /= 1) report "read_command(): identifier " &
                                         tmp_token.value_ptr.all & " found where should be an integer number." severity failure;
          assert (token_idx /= 4) report "read_command(): identifier " &
                                         tmp_token.value_ptr.all & " found where should be an { brace." severity failure;
          assert (token_idx /= 5) report "read_command(): identifier " &
                                         tmp_token.value_ptr.all & " found where should be an } brace or an integer number." severity failure;
          if (token_idx = 2) then
            if (tmp_token.value_ptr.all /= id) then -- Not our command, go to the end.
              while true loop
                read_token(f, tmp_token);
                exit when (tmp_token.kind = separator)or(tmp_token.value_ptr.all = "}");
              end loop;
              token_idx := 1;
            else
              token_idx := 3;
            end if;
          else
            deallocate(cmd.id);
            cmd.id := new string'(tmp_token.value_ptr.all);
            token_idx := 4;
          end if;

        when integer_number   =>
          assert (token_idx /= 2) report "read_command(): integer number " &
                                         tmp_token.value_ptr.all & " found where should be an identifier." severity failure;
          assert (token_idx /= 3) report "read_command(): integer number " &
                                         tmp_token.value_ptr.all & " found where should be an identifier." severity failure;
          assert (token_idx /= 4) report "read_command(): integer number " &
                                         tmp_token.value_ptr.all & " found where should be an { brace." severity failure;
          if (token_idx = 1) then
            cmd.time_cnt := to_integer(tmp_token.value_ptr.all);
            token_idx := 2;
          else
            tmp_array(arg_idx) := to_integer(tmp_token.value_ptr.all);
            arg_idx := arg_idx + 1;
          end if;

        when end_of_line      =>
          null;

        when end_of_file      =>
          exit;

        when start_of_comment =>
          while true loop
            read_token(f, tmp_token);
            exit when (tmp_token.kind = end_of_line)or(tmp_token.kind = end_of_file);
          end loop;

        when separator        =>
          if (token_idx = 4)and(tmp_token.value_ptr.all = "{") then
            token_idx := 5;
          elsif (token_idx = 5)and(tmp_token.value_ptr.all = "}") then
            token_idx := 6;
            exit;
          end if;

        when not_recognized   =>
          assert false report "read_command(): unknown pattern " & tmp_token.value_ptr.all & " found." severity failure;
      end case;
    end loop;

    assert (token_idx = 1)or(token_idx = 6) report "read_command(): Unexpected End of File." severity failure;

    if (token_idx = 1) then
      cmd.time_cnt := 0;
      deallocate(cmd.id);
      cmd.id := new string'("stop");
      deallocate(cmd.arg);
    else
      deallocate(cmd.arg);
      cmd.arg := new integer_array'(tmp_array(1 to arg_idx - 1));
    end if;
  end procedure read_command;
  ------------------------------------------------------------------------------


  ------------------------------------------------------------------------------
  procedure put_command(variable clist : inout command_list; cmd : inout command) is
    variable new_le : command_list_elem_ptr;
    variable tmp    : command_list_elem_ptr;
  begin
    new_le                  := new command_list_elem;
    new_le.all.cmd.time_cnt := cmd.time_cnt;
    new_le.all.cmd.id       := new string'(cmd.id.all);
    new_le.all.cmd.arg      := new integer_array'(cmd.arg.all);

    if (clist.number = 0) then
      -- The command list is empty.
      new_le.all.prv      := null;
      new_le.all.nxt      := null;
      clist.first         := new_le;
      clist.last          := new_le;
    else
      -- The command list is not empty.
      new_le.all.prv      := clist.last;
      new_le.all.nxt      := null;
      clist.last.all.nxt  := new_le;
      clist.last          := new_le;
    end if;
    clist.number   := clist.number + 1;
  end procedure put_command;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  procedure init_command_list(fname : in string; variable clist : inout command_list; block_id : in string) is
    file     cmd_data : char_file;
    variable cmd      : command;
  begin
    file_open(cmd_data, fname, read_mode);

    clist.number   := 0;
    clist.first    := null;
    clist.last     := null;

    while (true) loop
      read_command(cmd_data, block_id, cmd);
      exit when (cmd.id.all = "stop");
      put_command(clist, cmd);
    end loop;

    deallocate(cmd.id);
    deallocate(cmd.arg);
    file_close(cmd_data);
  end procedure init_command_list;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  procedure get_command(variable clist : inout command_list; cmd : inout command) is
    variable old_el : command_list_elem_ptr;
  begin
    deallocate(cmd.id);
    deallocate(cmd.arg);
    if (clist.number = 0) then
      cmd.time_cnt := 0;
      cmd.id       := new string'("stop");
    else
      if (clist.number = 1) then
        old_el         := clist.first;
        clist.number   := clist.number - 1;
        clist.first    := null;
        clist.last     := null;
      else
        old_el         := clist.first;
        clist.number   := clist.number - 1;
        clist.first    := clist.first.all.nxt;
        old_el.all.nxt.all.prv := null;
      end if;
      cmd.time_cnt := old_el.all.cmd.time_cnt;
      cmd.id       := new string'(old_el.all.cmd.id.all);
      cmd.arg      := new integer_array'(old_el.all.cmd.arg.all);
      deallocate(old_el.all.cmd.id);
      deallocate(old_el.all.cmd.arg);
      deallocate(old_el);
    end if;
  end procedure get_command;
  ------------------------------------------------------------------------------




  ------------------------------------------------------------------------------
  function to_string(value  : in std_logic;
                     format : in string  := "b";
                     width  : in natural := 0;
                     prec   : in natural := 0) return string is
  begin
    return to_string("" & value, format, width, prec);
  end function to_string;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  function sel(arg0 : boolean; arg1 : integer; arg2 : integer) return integer is
  begin
    if (arg0) then return arg1; else return arg2; end if;
  end function sel;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  function sel(arg0 : boolean; arg1 : std_logic; arg2 : std_logic) return std_logic is
  begin
    if (arg0) then return arg1; else return arg2; end if;
  end function sel;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  function sel(arg0 : boolean; arg1 : character; arg2 : character) return character is
  begin
    if (arg0) then return arg1; else return arg2; end if;
  end function sel;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  function max(arg0 : integer; arg1 : integer) return integer is
  begin
    return sel(arg0 > arg1, arg0, arg1);
  end function max;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  function signed_num_bits(arg : integer) return positive is
    variable nbits : natural := 1;
    variable n     : natural := sel(arg < 0, -(arg + 1), arg);
  begin
    while (n > 0) loop
      nbits := nbits + 1;
      n     := n / 2;
    end loop;
    return nbits;
  end function signed_num_bits;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  function to_string(value  : in integer;
                     format : in string  := "d";
                     width  : in natural := 0;
                     prec   : in natural := 0) return string is
    variable tmp      : std_logic_vector(signed_num_bits(integer'high) - 1 downto 0);
    variable negative : std_logic := sel(value < 0, '1', '0');
    variable n        : natural   := sel(value < 0, -(value + 1), value);
  begin
    for i in tmp'right to tmp'left loop
      tmp(i) := sel(((n mod 2) = 0), negative, not(negative));
      n      := n / 2;
    end loop;
    return to_string(tmp, format, width, prec);
  end function to_string;
  ------------------------------------------------------------------------------

  ------------------------------------------------------------------------------
  function to_string(value  : in std_logic_vector;
                     format : in string  := "x";
                     width  : in natural := 0;
                     prec   : in natural := 0) return string is
    -- Flags:
    variable minus_flag             : boolean := false;
    variable plus_flag              : boolean := false;
    variable space_flag             : boolean := false;
    variable zero_flag              : boolean := false;
    variable sharp_flag             : boolean := false;
    -- Width:
    variable width_local            : natural := 0;
    -- Precision:
    variable prec_local             : natural := 0;
    -- Conversion Type:
    type conversion_type_type is (unsigned_binary, signed_decimal, unsigned_octal, unsigned_decimal, unsigned_hexadecimal_0, unsigned_hexadecimal_1);
    variable type_local             : conversion_type_type := signed_decimal;
    --
    --
    variable value_local            : std_logic_vector(value'length - 1 downto 0);
    variable idx                    : integer;
    variable number_buffer          : string(1 to value'length);
    variable sign_buffer            : string(1 to 2);
    variable result_ptr             : string_ptr;
    --
    variable space_padding_0        : integer;
    variable sign_chars             : integer;
    variable zero_padding           : integer;
    variable number_chars           : integer;
    variable space_padding_1        : integer;
  begin
    idx := format'low;

    -- Parse format string:
    ---- Read flags:
    while (true) loop
      case format(idx) is
        when '-' =>
          if (minus_flag) then report "'-' flag was aready set before" severity warning; else minus_flag := true; end if;
        when '+' =>
          if (plus_flag ) then report "'+' flag was aready set before" severity warning; else plus_flag  := true; end if;
        when ' ' =>
          if (space_flag) then report "' ' flag was aready set before" severity warning; else space_flag := true; end if;
        when '0' =>
          if (zero_flag ) then report "'0' flag was aready set before" severity warning; else zero_flag  := true; end if;
        when '#' =>
          if (sharp_flag) then report "'#' flag was aready set before" severity warning; else sharp_flag := true; end if;
        when others => exit;
      end case;
      if (idx < format'high) then idx := idx + 1; else report "Unexpected end of format string." severity error; end if;
    end loop;

    if (space_flag and plus_flag) then
      report "' ' flag is ignored due to presence of '+' flag" severity warning;
      space_flag := false;
    end if;

    if (minus_flag and zero_flag) then
      report "'0' flag is ignored due to presence of '-' flag" severity warning;
      zero_flag := false;
    end if;

    ---- Read width:
    if (format(idx) = '*') then
      width_local     := width;
      if (idx < format'high) then idx := idx + 1; else report "Unexpected end of format string." severity error; end if;
    else
      while (true) loop
        case format(idx) is
          when '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' =>
            width_local := (width_local * 10) + (character'pos(format(idx)) - character'pos('0'));
            if (idx < format'high) then idx := idx + 1; else report "Unexpected end of format string." severity error; end if;
          when others => exit;
        end case;
      end loop;
    end if;

    ---- Read precision:
    if (format(idx) = '.') then
      prec_local     := 0;
      if (zero_flag) then
        report "'0' flag is ignored due to presence of PREC field" severity warning;
        zero_flag := false;
      end if;
      if (idx < format'high) then idx := idx + 1; else report "Unexpected end of format string." severity error; end if;
      if (format(idx) = '*') then
        prec_local     := prec;
        if (idx < format'high) then idx := idx + 1; else report "Unexpected end of format string." severity error; end if;
      else
        while (true) loop
          case format(idx) is
            when '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' =>
              prec_local := (prec_local * 10) + (character'pos(format(idx)) - character'pos('0'));
              if (idx < format'high) then idx := idx + 1; else report "Unexpected end of format string." severity error; end if;
            when others => exit;
          end case;
        end loop;
      end if;
    end if;

    ---- Read type:
    case format(idx) is
      when 'b'       =>
        type_local     := unsigned_binary;
      when 'd' | 'i' =>
        type_local     := signed_decimal;
        if (sharp_flag) then
          report "'#' flag is ignored due to signed decimal conversion type" severity warning;
          sharp_flag := false;
        end if;
      when 'o'       =>
        type_local     := unsigned_octal;
      when 'u'       =>
        type_local     := unsigned_decimal;
        if (sharp_flag) then
          report "'#' flag is ignored due to unsigned decimal conversion type" severity warning;
          sharp_flag := false;
        end if;
      when 'x'       =>
        type_local     := unsigned_hexadecimal_0;
      when 'X'       =>
        type_local     := unsigned_hexadecimal_1;
      when others    =>
        report "Unknown conversion type character: " & format(idx) severity error;
        type_local     := signed_decimal;
    end case;
    assert (idx = format'high) report "Extra characters in format string" severity warning;

    ---- Obtain number string:
    case type_local is
      when unsigned_binary =>
        -- Fill number buffer:
        value_local := value;
        idx         := 1;

        while (true) loop
          number_buffer(idx) := to_character(value_local(0));
          value_local := '0' & value_local(value_local'high downto 1);
          idx := idx + 1;
          exit when (value_local = (value_local'range => '0'));
        end loop;

        -- Fill sign buffer:
        if (sharp_flag) then
          sign_chars     := 1;
          sign_buffer(1) := 'b';
        else
          sign_chars      := 0;
        end if;

      when signed_decimal         =>
        -- Fill number buffer:
        value_local := std_logic_vector(abs(signed(value)));
        idx         := 1;

        while (true) loop
          number_buffer(idx) := hexadecimal_chars(to_integer(unsigned(value_local) rem 10) + 1);
          value_local := std_logic_vector(unsigned(value_local) / 10);
          idx := idx + 1;
          exit when (value_local = (value_local'range => '0'));
        end loop;

        -- Fill sign buffer:
        if (signed(value) < 0) then
          sign_buffer(1) := '-';
        elsif (plus_flag) then
          sign_buffer(1) := '+';
        else
          sign_buffer(1) := ' ';
        end if;
        sign_chars      := sel(signed(value) < 0, 1, sel(plus_flag or space_flag, 1, 0));

      when unsigned_octal         =>
        -- Fill number buffer:
        value_local := value;
        idx         := 1;

        while (true) loop
          number_buffer(idx) := hexadecimal_chars(to_integer(unsigned(value_local) rem 8) + 1);
          value_local := std_logic_vector(unsigned(value_local) / 8);
          idx := idx + 1;
          exit when (value_local = (value_local'range => '0'));
        end loop;

        -- Fill sign buffer:
        sign_chars      := 0;
        prec_local      := sel(sharp_flag and (prec_local < idx), idx, prec_local);

      when unsigned_decimal       =>
        -- Fill number buffer:
        value_local := value;
        idx         := 1;

        while (true) loop
          number_buffer(idx) := hexadecimal_chars(to_integer(unsigned(value_local) rem 10) + 1);
          value_local := std_logic_vector(unsigned(value_local) / 10);
          idx := idx + 1;
          exit when (value_local = (value_local'range => '0'));
        end loop;

        -- Fill sign buffer:
        sign_chars      := 0;

      when unsigned_hexadecimal_0 | unsigned_hexadecimal_1 =>
        -- Fill number buffer:
        value_local := value;
        idx         := 1;

        while (true) loop
          number_buffer(idx) := hexadecimal_chars(to_integer(unsigned(value_local) rem 16) + sel(type_local = unsigned_hexadecimal_0, 1, 17));
          value_local := std_logic_vector(unsigned(value_local) / 16);
          idx := idx + 1;
          exit when (value_local = (value_local'range => '0'));
        end loop;

        -- Fill sign buffer:
        if (sharp_flag) then
          sign_chars     := 2;
          sign_buffer(1) := '0';
          sign_buffer(2) := sel(type_local = unsigned_hexadecimal_0, 'x', 'X');
        else
          sign_chars      := 0;
        end if;

    end case;

    --
    number_chars    := idx - 1;
    zero_padding    := max(prec_local - number_chars, 0);
    space_padding_0 := sel(minus_flag, 0, max(width_local - (sign_chars + zero_padding + number_chars), 0));
    space_padding_1 := sel(minus_flag, max(width_local - (sign_chars + zero_padding + number_chars), 0), 0);

    --
    result_ptr := new string(1 to space_padding_0 + sign_chars + zero_padding + number_chars + space_padding_1);

    -- fill the space padding 0:
    for i in 1 to space_padding_0 loop
      result_ptr.all(i) := ' ';
    end loop;
    -- put sign chars:
    for i in 1 to sign_chars loop
      result_ptr.all(space_padding_0 + i) := sign_buffer(i);
    end loop;
    -- fill zero poadding:
    for i in 1 to zero_padding loop
      result_ptr.all(space_padding_0 + sign_chars + i) := '0';
    end loop;
    -- put number chars:
    for i in 1 to number_chars loop
      result_ptr.all(space_padding_0 + sign_chars + zero_padding + i) := number_buffer(idx - i);
    end loop;
    -- fill the space padding 1:
    for i in 1 to space_padding_1 loop
      result_ptr.all(space_padding_0 + sign_chars + zero_padding + number_chars + i) := ' ';
    end loop;

    return result_ptr.all;

  end function to_string;
  ------------------------------------------------------------------------------

end test;
--==============================================================================

