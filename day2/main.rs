use std::io;
use std::str::FromStr;

fn is_invalid(val : u64) -> bool {
    let val_str = val.to_string();
    
    if val_str.len() % 2 == 0 {
        let mid = val_str.len() / 2;
        let (left, right) = val_str.split_at(mid);
        return left == right;
    }

    return false;
}

fn is_invalid_b(val: u64) -> bool {
    let val_str = val.to_string();

    for i in 2..=val_str.len() {
        if val_str.len() % i != 0 {
            continue;
        }
        let seq_size = val_str.len() / i;
        let mut is_invalid = true;
        for j in 0..i-1 {
            let seq1 = &val_str[j * seq_size..(j + 1) * seq_size];
            let seq2 = &val_str[(j + 1) * seq_size..(j + 2) * seq_size];
            if seq1 != seq2 {
                is_invalid = false;
                break;
            }
        }
        if is_invalid {
            return true;
        }
    }

    return false;
}

fn main() {
    let mut input = String::new();
    io::stdin().read_line(&mut input).unwrap();
    let ranges: Vec<&str> = input.split(',').collect();

    let mut res_a: u64 = 0;
    let mut res_b: u64 = 0;
    for range in ranges {
        let mut parts = range.split('-');
        let a = parts.next().unwrap();
        let b = parts.next().unwrap();
        let a_val = u64::from_str(&a).unwrap();
        let b_val = u64::from_str(&b).unwrap();

        for val in a_val..=b_val {
            if is_invalid(val) {
                res_a += val;
            }
            if is_invalid_b(val) {
                res_b += val;
            }
        }
    }

    println!("{res_a}");
    println!("{res_b}");
}