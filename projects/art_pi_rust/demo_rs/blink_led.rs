#![no_std]

use rtt_rs::*;
use driver_for_rtt as device;
use device::api::{find, DevOpen, OpenFlag};
use device::driver::DriverOps;
use device::device::led::LedState;

entry!(start);
fn start() {
    let _ = thread::Thread::new().start(blink_led).unwrap();
    println!("blink start!");
}

fn blink_led() {
    println!("blink!");
    let led = find("led0").unwrap();
    log!("blink find!");
    let led_open = led.open(&OpenFlag::zero()).unwrap();
    log!("start!");
    loop {
        led_open.write(0, &LedState::On).unwrap();
        thread::Thread::delay(500);
        led_open.write(0, &LedState::Off).unwrap();
        thread::Thread::delay(500);
    }
}