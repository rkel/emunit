#ifndef CONFIG_EMUNIT_H_INCLUDED
#define CONFIG_EMUNIT_H_INCLUDED
/* EMUnit embedded unit test engine - Copyright (C) 2017 Radoslaw Koppel
 *
 * This program is free software: you can redistribute it and/or modify
 * in under the terms of the GNU General Public license (version 3)
 * as published by the Free Software Foundation AND MODIFIED BY the
 * EMUnit exception.
 *
 * NOTE: The exception was added to the GPL to ensure
 * that you can test any kind of software without being
 * obligated to release the whole source code under the terms of GPL.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * and the EMUnit license exception along with this program.
 * If not, it can be viewed in the original EMunit repository:
 * <https://github.com/rkel/emunit>.
 */
/**
 * @file
 * @brief EMUnit configuration
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 * File with EMUnit configuration.
 * @sa config_emunit_group
 */

/**
 * @defgroup config_emunit_group <config_emunit> EMUnit configuration
 * @{
 * @ingroup emunit_group
 *
 * Default EMUnit configuration.
 * All values here can be overwritten by global definitions.
 * @{
 */

#ifndef EMUNIT_CONF_PORT
/**
 * @brief Selected port
 *
 * The name od the selected port.
 */
#define EMUNIT_CONF_PORT simavr
#endif

#ifndef EMUNIT_CONF_DISPLAY
/**
 * @brief Selected logger
 *
 * The name of the selected output logger.
 */
#define EMUNIT_CONF_DISPLAY xml
#endif

#ifndef EMUNIT_CONF_DISPLAY_NL
/**
 * @brief Display new line style
 *
 * The string used as a new line marker
 */
#define EMUNIT_CONF_DISPLAY_NL "\n"
#endif

#ifndef EMUNIT_CONF_BUFFER_SIZE
/**
 * @brief The size of internal buffer
 *
 * The size of the buffer that is filled when test is ongoing.
 * This buffer would be printed out when test is finished and MCU is restarted.
 */
#define EMUNIT_CONF_BUFFER_SIZE 512
#endif

#ifndef EMUNIT_CONF_NUMBER_SIZE
/**
 * @brief The maximum supported number size
 *
 * It has to be just a number because it would be connected to create
 * default type for numeric comparations.
 */
#define EMUNIT_CONF_NUMBER_SIZE 32
#endif

#ifndef EMUNIT_CONF_STRLEN_LIMIT
/**
 * @brief Maximum displayed string limitation
 *
 * Maximum number of string characters that would be presented when comparing
 * strings.
 */
#define EMUNIT_CONF_STRLEN_LIMIT 32
#endif

/** @} */
#endif /* CONFIG_EMUNIT_H_INCLUDED */
