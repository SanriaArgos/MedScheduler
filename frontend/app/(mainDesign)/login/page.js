"use client";

import Link from "next/link";
import { useState } from "react";
import { useRouter } from 'next/navigation'; // Импорт useRouter

export default function LoginPage() {
    const [phone, setPhone] = useState("");
    const [password, setPassword] = useState("");
    const [error, setError] = useState("");
    const [loading, setLoading] = useState(false);
    const router = useRouter(); // Инициализация router

    const handleSubmit = async (e) => {
        e.preventDefault();
        setError("");
        setLoading(true);

        if (!phone || !password) {
            setError("Номер телефона и пароль обязательны.");
            setLoading(false);
            return;
        }

        try {
            const response = await fetch('https://api.medscheduler.ru/login', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ phone, password }),
            })

            const data = await response.json();

            if (response.ok && data.success) {
                // Успешный вход
                console.log("Login successful", data);
                const [userType, userId] = data.result.split(':');

                localStorage.setItem('medSchedulerUser', JSON.stringify({
                    userId,
                    userType,
                    phone
                }));
                localStorage.setItem('isLoggedIn', 'true'); // Дополнительный флаг для простоты проверки

                // Перенаправляем на дашборд вместо главной страницы
                router.push('/dashboard');
            } else {
                // Ошибка со стороны API (например, неверные учетные данные) или ошибка сети
                setError(data.error || "Ошибка входа. Пожалуйста, попробуйте снова.");
            }
        } catch (err) {
            console.error("Login API error:", err);
            setError("Не удалось подключиться к серверу. Пожалуйста, проверьте ваше соединение.");
        } finally {
            setLoading(false);
        }
    };

    return (
        <div className="lg:min-h-[calc(90vh-theme(space.36))] flex flex-col items-center justify-center bg-gray-50 py-12 px-4 sm:px-6 lg:px-8">
            <div className="max-w-md w-full space-y-8 bg-white p-10 rounded-xl shadow-lg">
                <div>
                    <h2 className="mt-6 text-center text-3xl font-extrabold text-main2">
                        Вход в аккаунт
                    </h2>
                </div>
                <form className="mt-8 space-y-6" onSubmit={handleSubmit}>
                    <input type="hidden" name="remember" defaultValue="true" />
                    <div className="rounded-md shadow-sm -space-y-px">
                        <div>
                            <label htmlFor="phone" className="sr-only">
                                Номер телефона
                            </label>
                            <input
                                id="phone"
                                name="phone"
                                type="tel"
                                autoComplete="tel"
                                required
                                className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-t-md focus:outline-none focus:ring-main focus:border-main focus:z-10 sm:text-sm"
                                placeholder="Номер телефона"
                                value={phone}
                                onChange={(e) => setPhone(e.target.value)}
                            />
                        </div>
                        <div>
                            <label htmlFor="password" className="sr-only">
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
                    </div>

                    {error && (
                        <p className="text-sm text-red-600 text-center py-2">{error}</p>
                    )}

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

