<?php

class Instruction {

    static $set = [

        //             0     1     2     3     4     5     6     7     8     9     10    11    12
        //             IMM   ZP    ZPX   ZPY   ABS   ABSX  ABSY  I,X   I,Y   IMP   REL   ACC   IND
        'adc' => array(0x69, 0x65, 0x75,   -1, 0x6d, 0x7d, 0x79, 0x61, 0x71,   -1,   -1,   -1,   -1), /* 00 */
        'and' => array(0x29, 0x25, 0x35,   -1, 0x2d, 0x3d, 0x39, 0x21, 0x31,   -1,   -1,   -1,   -1), /* 01 */
        'asl' => array(  -1, 0x06, 0x16,   -1, 0x0e, 0x1e,   -1,   -1,   -1,   -1,   -1, 0x0a,   -1), /* 02 */
        'bcc' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x90,   -1,   -1), /* 03 */
        'bcs' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xb0,   -1,   -1), /* 04 */
        'beq' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xf0,   -1,   -1), /* 05 */
        'bit' => array(  -1, 0x24,   -1,   -1, 0x2c,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1), /* 06 */
        'bmi' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x30,   -1,   -1), /* 07 */
        'bne' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xd0,   -1,   -1), /* 08 */
        'bpl' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x10,   -1,   -1), /* 09 */
        'brk' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x00,   -1,   -1,   -1), /* 0A */
        'bvc' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x50,   -1,   -1), /* 0B */
        'bvs' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x70,   -1,   -1), /* 0C */
        'clc' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x18,   -1,   -1,   -1), /* 0D */
        'cld' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xd8,   -1,   -1,   -1), /* 0E */
        'cli' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x58,   -1,   -1,   -1), /* 0F */
        'clv' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xb8,   -1,   -1,   -1), /* 10 */
        'cmp' => array(0xc9, 0xc5, 0xd5,   -1, 0xcd, 0xdd, 0xd9, 0xc1, 0xd1,   -1,   -1,   -1,   -1), /* 11 */
        'cpx' => array(0xe0, 0xe4,   -1,   -1, 0xec,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1), /* 12 */
        'cpy' => array(0xc0, 0xc4,   -1,   -1, 0xcc,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1), /* 13 */
        'dec' => array(  -1, 0xc6, 0xd6,   -1, 0xce, 0xde,   -1,   -1,   -1,   -1,   -1,   -1,   -1), /* 14 */
        'dex' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xca,   -1,   -1,   -1), /* 15 */
        'dey' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x88,   -1,   -1,   -1), /* 16 */
        'eor' => array(0x49, 0x45, 0x55,   -1, 0x4d, 0x5d, 0x59, 0x41, 0x51,   -1,   -1,   -1,   -1), /* 17 */
        'inc' => array(  -1, 0xe6, 0xf6,   -1, 0xee, 0xfe,   -1,   -1,   -1,   -1,   -1,   -1,   -1), /* 18 */
        'inx' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xe8,   -1,   -1,   -1), /* 19 */
        'iny' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xc8,   -1,   -1,   -1), /* 1A */
        'jmp' => array(  -1,   -1,   -1,   -1, 0x4c,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x6c), /* 1B */
        'jsr' => array(  -1,   -1,   -1,   -1, 0x20,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1), /* 1C */
        'lda' => array(0xa9, 0xa5, 0xb5,   -1, 0xad, 0xbd, 0xb9, 0xa1, 0xb1,   -1,   -1,   -1,   -1), /* 1D */
        'ldx' => array(0xa2, 0xa6,   -1, 0xb6, 0xae,   -1, 0xbe,   -1,   -1,   -1,   -1,   -1,   -1), /* 1E */
        'ldy' => array(0xa0, 0xa4, 0xb4,   -1, 0xac, 0xbc,   -1,   -1,   -1,   -1,   -1,   -1,   -1), /* 1F */
        'lsr' => array(  -1, 0x46, 0x56,   -1, 0x4e, 0x5e,   -1,   -1,   -1,   -1,   -1, 0x4a,   -1), /* 20 */
        'nop' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xea,   -1,   -1,   -1), /* 21 */
        'ora' => array(0x09, 0x05, 0x15,   -1, 0x0d, 0x1d, 0x19, 0x01, 0x11,   -1,   -1,   -1,   -1), /* 22 */
        'pha' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x48,   -1,   -1,   -1), /* 23 */
        'php' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x08,   -1,   -1,   -1), /* 24 */
        'pla' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x68,   -1,   -1,   -1), /* 25 */
        'plp' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x28,   -1,   -1,   -1), /* 26 */
        'rol' => array(  -1, 0x26, 0x36,   -1, 0x2e, 0x3e,   -1,   -1,   -1,   -1,   -1, 0x2a,   -1), /* 27 */
        'ror' => array(  -1, 0x66, 0x76,   -1, 0x6e, 0x7e,   -1,   -1,   -1,   -1,   -1, 0x6a,   -1), /* 28 */
        'rti' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x40,   -1,   -1,   -1), /* 29 */
        'rts' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x60,   -1,   -1,   -1), /* 2A */
        'sbc' => array(0xe9, 0xe5, 0xf5,   -1, 0xed, 0xfd, 0xf9, 0xe1, 0xf1,   -1,   -1,   -1,   -1), /* 2B */
        'sec' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x38,   -1,   -1,   -1), /* 2C */
        'sed' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xf8,   -1,   -1,   -1), /* 2D */
        'sei' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x78,   -1,   -1,   -1), /* 2E */
        'sta' => array(  -1, 0x85, 0x95,   -1, 0x8d, 0x9d, 0x99, 0x81, 0x91,   -1,   -1,   -1,   -1), /* 2F */
        'stx' => array(  -1, 0x86,   -1, 0x96, 0x8e,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1), /* 30 */
        'sty' => array(  -1, 0x84, 0x94,   -1, 0x8c,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1), /* 31 */
        'tax' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xaa,   -1,   -1,   -1), /* 32 */
        'tay' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xa8,   -1,   -1,   -1), /* 33 */
        'tsx' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xba,   -1,   -1,   -1), /* 34 */
        'txa' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x8a,   -1,   -1,   -1), /* 35 */
        'txs' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x9a,   -1,   -1,   -1), /* 36 */
        'tya' => array(  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x98,   -1,   -1,   -1), /* 37 */
    ];

}

function parse_data( $s ) {
    
    $n = '';
    $quote = 0;
    $data = [];    
    for ($i = 0; $i < strlen($s); $i++) {
        
        $ch = $s[$i];
        if ($s[$i] == '"') {
            $quote = 1 - $quote;
            $n .= '"';
        } elseif ($quote) {
            $n .= $ch;        
        } elseif ($ch == ',') {        
            $data[] = $n;
            $n = '';
        } else {
            $n .= $ch;
        }
    }
    if ($n) $data[] = $n;
    
    foreach ($data as $value) {
        $value = trim($value);
        if ($value[0] == '"') {
            $value = substr($value, 1, -1);
            for ($j = 0; $j < strlen($value); $j++) {
                prg_insert(ord($value[$j]));
            }
        } else {
            echo "$value\n";
            prg_insert(prepare_numeric($value));
        }
    }    
}

// Превращение чисел к decimal
function prepare_numeric($s, $soft = false) {

    $s = trim($s);

    if (preg_match('~^(0x|\$|\&h)([0-9a-f]+)~i', $s, $c)) {
        return hexdec($c[2]);
    }

    if (preg_match('~^0b([01_]+)~i', $s, $c)) {
        return bindec(str_replace('_', '', $c[1]));
    }

    if (preg_match('~^\-?\s*[0-9\.]+~', $s, $c)) {
        return $c[0];
    }
    
    if (preg_match("~'(.)'~", $s, $c)) {
        return ord($c[1]);
    }

    if ($soft == false) {
        die("Unexpected number: $s\n");
    }

    return null;
}

// Вставка байт в код
function prg_insert() {

    $args = func_get_args();
    
    foreach ($args as $value) {

        $value &= 0xff;        
        // echo RT::$org." -- ".dechex($value)."\n";
        RT::$program[ RT::$org ] = $value;
        RT::$org++;
    }
}

/*
 * Ассемблирование линии. В линии уже должны быть расставлены
 * необходимые значения, раскрыты define и прочее.
 * Это чистое ассемблирование.
 *
 * [LABEL] <MnemonicName> [Operand]
 */

function assemble_line($word) {

    $s0     = strtolower($word[0]);
    $s1     = isset($word[1]) ? strtolower($word[1]) : '';
    $name   = '';

    // Первая точка - инструкция
    if (isset(Instruction::$set[ $s0 ])) {

        $label      = '';
        $name       = $s0;
        $rule       = Instruction::$set[ $s0 ];
        $operand    = join("", array_slice($word, 1));

    // Имеется метка
    } elseif (isset(Instruction::$set[ $s1 ])) {

        $label      = $word[0];
        $rule       = Instruction::$set[ $s1 ];
        $name       = $s1;
        $operand    = join("", array_slice($word, 2));

    // Одиночная метка
    } else {

        $label      = $word[0];
        $rule       = false;
    }

    /*
     * Поиск подходящих вариантов адресации
     */
     
    if ($label) {
        RT::$labels[ $label ] = RT::$org;        
    }

    // Только в случае получения RULE
    if ($rule) {

        // echo "[$label] $name -- $operand\n";        
        insert_by_rule($rule, $operand);
    }
}

function insert_by_rule($rule, $operand) {

    $data = [];
    $label = $operand;
    
    // hi(label), lo(label)
    if (preg_match('~^#?(hi|lo)\(([a-z_][a-z0-9]*)\)$~i', $operand, $cp)) {        
        
        RT::$branches[ RT::$org ] = [ strtoupper($cp[1]), $cp[2] ];
        $operand = '#$ff';
    }
    
    // Метка FAR
    elseif (strtolower($operand) != 'a' && preg_match('~^[a-z_][a-z0-9]*$~i', $operand)) {
        
        RT::$branches[ RT::$org ] = ['FAR', $operand];
        $operand = '$ffff';
    }
    
    // @todo Определить в операндах наличие метки

    // Это Immediate?
    if (preg_match('~^#(.+)$~', $operand, $x)) {
        $data[0] = prepare_numeric($x[1]);
    }
    // Это Indirect,X?
    else if (preg_match('~^\((.+),x\)$~i', $operand, $x)) {
        $data[7] = prepare_numeric($x[1]);
    }
    // Это Indirect,Y?
    else if (preg_match('~^\((.+)\),y$~i', $operand, $x)) {
        $data[8] = prepare_numeric($x[1]);
    }
    // ZP,X | ABS,X
    else if (preg_match('~^(.+),x$~i', $operand, $x)) {

        $value = prepare_numeric($x[1]);
        if ($value < 0x100) {
            $data[2] = $value;
        } else {
            $data[5] = $value;
        }
    }
    // ZP,Y | ABS,Y
    else if (preg_match('~^(.+),y$~i', $operand, $x)) {
        $value = prepare_numeric($x[1]);
        if ($value < 0x100) {
            $data[3] = $value;
        } else {
            $data[6] = $value;        
        }   
    }
    // Это (IND)?
    else if (preg_match('~^\((.+)\)$~i', $operand, $x)) {
        $data[12] = prepare_numeric($x[1]);
    }
    // Это A?
    else if (preg_match('~^a$~i', $operand, $x)) {
        $data[11] = 0;
    }
    // Implied
    else if ($operand == '') {
        $data[9] = 0;
    } else {
        $value = prepare_numeric($operand, true);
        if ($value < 0x100) {
            $data[1] = $value;
        } else {
            $data[4] = $value;
        }
        $data[10] = $operand;
    }
    
    // Исключить неправильные опкоды и не входящие в $data / $rule
    foreach ($rule as $Id => $opcode) {
        
        if (!isset($data[ $Id ])) {
            unset($rule[ $Id ]);
            continue;
        }
        
        if ($opcode < 0) {
            unset($rule[ $Id ]);
        }
    }

    // Совместить с $rule
    foreach ($rule as $operand_id => $opcode) {
        
        $value = $data[ $operand_id ];
        switch ($operand_id) {
            
            case 0: // IMM
            case 1: // ZP
            case 2: // ZP,X
            case 3: // ZP,Y
            case 7: // NDX
            case 8: // NDY
            
                return prg_insert($opcode, $value);

            case 4: // ABS
            case 5: // ABX
            case 6: // ABZ
            case 12: // IND
            
                return prg_insert($opcode, $value, $value >> 8);

            case 10: // LABEL
            
                RT::$branches[ RT::$org ][0] = 'NEAR';
                return prg_insert($opcode, 0); 
                        
            case 9:  // IMP
            case 11: // ACC

                return prg_insert($opcode);             
        }    
    }
    
    die("Operand not match: $operand\n");    
}
