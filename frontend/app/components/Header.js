"use client";

import Link from "next/link";
import Image from "next/image";
import { useState, useEffect } from "react";
import { usePathname } from 'next/navigation';
import { SearchBar } from "./SearchBar";

export const Header = () => {
    const [isLoggedIn, setIsLoggedIn] = useState(false);
    const [userType, setUserType] = useState(null);
    const pathname = usePathname();

    useEffect(() => {
        // Проверяем статус авторизации при монтировании компонента
        const loginStatus = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');
        setIsLoggedIn(loginStatus === 'true');
        setUserType(userData.userType || null);
    }, [pathname]); // Повторно проверяем при изменении URL

    return <header className="w-full h-14 xl:h-20 z-[100] fixed top-0 left-0 bg-main">
        <div className="w-full h-full max-w-screen-2xl mx-auto flex items-center justify-between px-2.5">
            <Link href="/" className="flex items-center h-full shrink-0">
                <Image src="/icon-512.png" alt="MedScheduler Logo" width={48} height={48} className="h-10 w-10 xl:h-12 xl:w-12"/>
                <span className="ml-2 text-sm xl:text-2xl font-bold text-white">MedScheduler</span>
            </Link>
            <nav className="w-fit h-full flex gap-2 xl:gap-6 font-medium text-[15px] xl:text-[20px] items-center leading-none text-white">
                <Link href={"/"} className="hidden lg:block hover:text-main2 transition-colors">Главная</Link>
                <Link href={"/search"} className="hover:text-main2 transition-colors">Поиск</Link>
                <Link href={"/#about"} className="hover:text-main2 transition-colors">О нас</Link>
                <Link href={"/site-map"} className="hidden lg:block hover:text-main2 transition-colors">Карта сайта</Link>
                <Link href={"/site-map"} className="lg:hidden hover:text-main2 transition-colors">Карта<br/>сайта</Link>
                <Link href={"/api-documentation"} className="hover:text-main2 transition-colors">API</Link>

                {isLoggedIn ? (
                    <>
                        <Link href={"/dashboard"} className="hover:text-main2 transition-colors">
                            <span className="hidden lg:block">Личный кабинет</span><span className="lg:hidden">ЛК</span>
                        </Link>
                        {userType === 'patient' && (
                            <Link href={"/patient/appointments"} className="hover:text-main2 transition-colors">
                                <span className="hidden lg:block">Мои записи</span><span className="lg:hidden">Записи</span>
                            </Link>
                        )}
                    </>
                ) : (
                    <Link href={"/login"} className="bg-main2 hover:bg-opacity-80 transition-all text-white px-4 py-2 rounded-md">Войти</Link>
                )}
            </nav>
        </div>
    </header>
}

