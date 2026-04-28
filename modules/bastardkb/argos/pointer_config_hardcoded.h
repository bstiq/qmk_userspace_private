/*
    For now we redefine some stuff that's already in dilemma.c
    The module gets compiled first, so we don't have access to the stuff from dilemma
*/

#pragma once

#define ARGOS_DILEMMA_MINIMUM_DEFAULT_DPI 400
#define ARGOS_DILEMMA_DEFAULT_DPI_CONFIG_STEP 200
#define ARGOS_DILEMMA_MINIMUM_SNIPING_DPI 200
#define ARGOS_DILEMMA_SNIPING_DPI_CONFIG_STEP 100
#define ARGOS_DILEMMA_DRAGSCROLL_DPI 100
#define ARGOS_DILEMMA_DRAGSCROLL_BUFFER_SIZE 6