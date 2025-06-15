"use client";

import { useState, useEffect } from "react";
import { useRouter } from "next/navigation";
import Link from "next/link";
import { formatPhoneDisplay, formatPhoneForAPI, validatePhone } from '@/utils/phoneFormatter';

export default function LoginPage() {
    const [phone, setPhone] = useState("");
    const [password, setPassword] = useState("");
    const [error, setError] = useState("");
    const [loading, setLoading] = useState(false);
    const router = useRouter();

    useEffect(() => {
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        if (isLoggedIn) {
            const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');
            redirectToDashboard(userData.userType);
        }
    }, [router]);

    const redirectToDashboard = (userType) => {
        switch (userType) {
            case "patient":
                router.push("/patient/profile");
                break;
            case "doctor":
                router.push("/doctor/profile");
                break;
            case "junior_admin":
                router.push("/junior-admin/profile");
                break;
            case "senior_admin":
                router.push("/senior-admin/profile");
                break;
            default:
                router.push("/"); // Fallback
        }
    };

    const handleSubmit = async (e) => {
        e.preventDefault();
        setLoading(true);
        setError("");

        if (!validatePhone(phone)) {
            setError("Введите корректный  номер телефона.");
            setLoading(false);
            return;
        }

        const formattedPhoneForAPI = formatPhoneForAPI(phone);

        try {
            const response = await fetch("https://api.medscheduler.ru/login", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ phone: formattedPhoneForAPI, password }),
            })

            const data = await response.json();

            if (response.ok && data.success) {
                // Успешный вход
                console.log("Login successful", data);
                const [userType, userId] = data.result.split(':');

                // Сохраняем базовую информацию о пользователе
                localStorage.setItem('medSchedulerUser', JSON.stringify({
                    userId,
                    userType,
                    phone
                }));
                localStorage.setItem('isLoggedIn', 'true');

                // Перенаправляем на дашборд
                router.push('/dashboard');
            } else {
                // Ошибка со стороны API
                setError(data.error || "Неверный логин или пароль. Пожалуйста, попробуйте снова.");
            }
        } catch (err) {
            console.error("Login API error:", err);
            setError("Не удалось подключиться к серверу. Пожалуйста, проверьте ваше соединение.");
        } finally {
            setLoading(false);
        }
    };

    const handlePhoneChange = (e) => {
        // Сразу форматируем для отображения
        setPhone(formatPhoneDisplay(e.target.value));
    };

    return (
        // Убираем градиент, возвращаем более простой фон, например, светло-серый
        <div className="min-h-screen flex items-center justify-center bg-gray-100">
            <div className="bg-white p-8 rounded-xl shadow-2xl w-full max-w-md">
                <h1 className="text-3xl font-bold text-center text-main2 mb-8">
                    Вход в систему
                </h1>
                {error && (
                    <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-6">
                        {error}
                    </div>
                )}
                <form onSubmit={handleSubmit} className="space-y-6">
                    <div>
                        <label
                            htmlFor="phone"
                            className="block text-sm font-medium text-gray-700 mb-1"
                        >
                            Номер телефона
                        </label>
                        <input
                            type="tel"
                            id="phone"
                            name="phone"
                            value={phone} // Используем уже отформатированное значение из состояния
                            onChange={handlePhoneChange}
                            // Применяем стили, аналогичные другим полям ввода
                            className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-t-md focus:outline-none focus:ring-main focus:border-main sm:text-sm"
                            placeholder="+7 (999) 000-00-00"
                            required
                        />
                    </div>
                    <div>
                        <label htmlFor="password" className="block text-sm font-medium text-gray-700 mb-1">
                            Пароль
                        </label>
                        <input
                            id="password"
                            name="password"
                            type="password"
                            autoComplete="current-password"
                            required
                            className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-b-md focus:outline-none focus:ring-main focus:border-main focus:z-10 sm:text-sm"
                            placeholder="Пароль"
                            value={password}
                            onChange={(e) => setPassword(e.target.value)}
                        />
                    </div>
                    <div>
                        <button
                            type="submit"
                            disabled={loading}
                            className="group relative w-full flex justify-center py-2 px-4 border border-transparent text-sm font-medium rounded-md text-white bg-main hover:bg-main2 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-main2 transition-colors disabled:opacity-50"
                        >
                            {loading ? "Вход..." : "Войти"}
                        </button>
                    </div>
                </form>
                <div className="text-sm text-center">
                    <p className="text-gray-600">
                        Нет аккаунта?{' '}
                        <Link href="/registration" className="font-medium text-main hover:text-main2">
                            Зарегистрироваться
                        </Link>
                    </p>
                </div>
            </div>
        </div>
    );
}

