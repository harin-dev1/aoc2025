use std::io;

fn main() {
    let mut rotation = String::new();
    let mut position = 50;
    let mut res_a : u64 = 0;
    let mut res_b : u64 = 0;

    loop {
        rotation.clear();
        if io::stdin().read_line(&mut rotation).is_err() || rotation.trim().is_empty() {
            break;
        }
        
        let rotation_value: i32 = rotation[1..].trim().parse().expect("Invalid number in rotation");
        let first_char = rotation.chars().nth(0).expect("Rotation string is empty");
        if first_char == 'L' {
            position -= rotation_value;
        } else {
            position += rotation_value;
        }

        res_b += position.abs() as u64 / 100;
        if position <= 0 && position.abs() != rotation_value {
            res_b += 1;
        }
        position = (position % 100 + 100) % 100;
        if position == 0 {
            res_a += 1;
        }
    }

    println!("{res_a}");
    println!("{res_b}");
}