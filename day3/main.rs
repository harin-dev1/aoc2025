use std::io::{self, BufRead};

fn get_largest_possible_joltage(bank : &String) -> u64 {
    let largest_possible_joltage : u64;

    // There isn't a standard crate specifically for finding the maximum element and its index in a string,
    // but using the `itertools` crate's `max_by_key_position` would simplify element-with-index tracking.
    // For std-only, use .char_indices().max_by_key(|(_, c)| *c):

    let mut max_digit = '0';
    let mut max_index = 0;
    for (i, c) in bank.char_indices().take(bank.len() - 1) {
        if c > max_digit {
            max_digit = c;
            max_index = i;
        }
    }

    let max_digit_u64 = max_digit.to_digit(10).unwrap() as u64;
    // Find the max digit starting from max_index + 1
    let next_max_digit = bank
        .chars()
        .skip(max_index + 1)
        .max()
        .unwrap_or('0');
    let next_max_digit_u64 = next_max_digit.to_digit(10).unwrap() as u64;
    largest_possible_joltage = max_digit_u64 * 10 + next_max_digit_u64;
    return largest_possible_joltage;
}

const NUM_BATTERIES : usize = 12;

fn get_largest_possible_joltage_index(bank : &String, battery_index : usize,
    start_index : usize) -> usize {
    let num_remaining = bank.len() - start_index;
    let max_window_len = num_remaining - (NUM_BATTERIES - battery_index);

    let mut max_digit = '0';
    let mut max_index = 0;
    for i in 0..max_window_len {
        if bank.chars().nth(start_index + i).unwrap() > max_digit {
            max_digit = bank.chars().nth(start_index + i).unwrap();
            max_index = start_index + i;
        }
    }

    max_index
}

fn largest_possible_joltage_2(bank : &String) -> u64 {
    let mut largest_possible_joltage : u64 = 0;
    let mut start_index : usize = 0;
    for i in 1..=NUM_BATTERIES {
        let index = get_largest_possible_joltage_index(bank, i, start_index);
        start_index = index + 1;
        largest_possible_joltage = largest_possible_joltage * 10 +
                                        bank.chars().nth(index).unwrap().to_digit(10).unwrap() as u64;
    }

    largest_possible_joltage
}

fn main() {
    let stdin = io::stdin();
    let mut res_a : u64 = 0;
    let mut res_b : u64 = 0;
    for line in stdin.lock().lines() {
        let line = line.unwrap();
        res_a += get_largest_possible_joltage(&line);
        res_b += largest_possible_joltage_2(&line);
    }

    println!("{res_a}");
    println!("{res_b}");
}