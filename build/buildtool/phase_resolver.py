#! /usr/bin/env python
# -*- coding: utf-8 -*-

def resolve(phase_depend_map, input_phases):
    u"""根据phase间的依赖关系，对用户输入进行解析
    解析结果返回值为 (invalid_phases, valid_phases)
    invalid_phases 的值与用户输入的顺序相同
    valid_phases则不保证
    valid_phases可能包含用户未输入的phase。这些phase是被依赖到的phase
    """

    phase_to_run = dict.fromkeys(phase_depend_map.keys(), False)

    invalid_phases = [p for p in input_phases
        if not phase_depend_map.has_key(p)]

    valid_input = [p for p in input_phases
        if phase_depend_map.has_key(p)]

    for phase in valid_input:
        for phase_depend in phase_depend_map[phase]:
            phase_to_run[phase_depend] = True
        phase_to_run[phase] = True

    valid_phases = [p for p in phase_to_run if phase_to_run[p]]
    return (invalid_phases, valid_phases)
