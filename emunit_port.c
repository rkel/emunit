/**
 * @file
 * @brief EMUnit port source code
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 * This file just includes the right source file form the selected port.
 * Nothing more done here.
 * It is here only to simplify the compilation rules.
 * @sa emunit_port_group
 */
#include "emunit_port.h"

/* Include the source code for selected port */
#include EMUNIT_PORT_FILE( , c)
