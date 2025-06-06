"use client";

import { useState, useEffect } from "react";
import { useRouter } from 'next/navigation';
import Link from "next/link";

export default function DashboardPage() {
    const [loading, setLoading] = useState(true);
    const [userData, setUserData] = useState(null);
    const [error, setError] = useState("");
    const router = useRouter();

    useEffect(() => {
        // Проверка аутентификации
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const storedUserData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        if (!isLoggedIn || !storedUserData.userId) {
            router.push('/login');
            return;
        }

        setUserData(storedUserData);
        setLoading(false);
    }, [router]);

    const handleLogout = () => {
        localStorage.removeItem('medSchedulerUser');
        localStorage.removeItem('isLoggedIn');
        router.push('/login');
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка...</div>
            </div>
        );
    }

    // Компоненты панели для разных типов пользователей
    const PatientDashboard = () => (
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
            <Link href="/patient/profile" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Профиль</h3>
                <p className="text-gray-600">Просмотр и редактирование личных данных</p>
            </Link>
            <Link href="/search" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Поиск врачей</h3>
                <p className="text-gray-600">Найдите нужного специалиста и запишитесь на прием</p>
            </Link>
            <Link href="/patient/appointments" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Мои записи</h3>
                <p className="text-gray-600">Просмотр и управление записями на прием</p>
            </Link>
            <Link href="/patient/waitlist" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Лист ожидания</h3>
                <p className="text-gray-600">Просмотр позиций в листе ожидания</p>
            </Link>
        </div>
    );

    const DoctorDashboard = () => (
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
            <Link href="/doctor/profile" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Профиль</h3>
                <p className="text-gray-600">Просмотр и редактирование профессиональных данных</p>
            </Link>
            <Link href="/doctor/schedule" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Мое расписание</h3>
                <p className="text-gray-600">Просмотр расписания и записей пациентов</p>
            </Link>
        </div>
    );

    const JuniorAdminDashboard = () => (
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
            <Link href="/junior-admin/profile" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Профиль</h3>
                <p className="text-gray-600">Просмотр и редактирование данных администратора</p>
            </Link>
            <Link href="/junior-admin/doctors" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Управление врачами</h3>
                <p className="text-gray-600">Добавление и прикрепление врачей к больнице</p>
            </Link>
            <Link href="/junior-admin/schedule" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Управление расписанием</h3>
                <p className="text-gray-600">Редактирование расписания врачей</p>
            </Link>
            <Link href="/junior-admin/waitlist" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Лист ожидания</h3>
                <p className="text-gray-600">Просмотр листа ожидания</p>
            </Link>
            <Link href="/junior-admin/doctor-stats" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Статистика</h3>
                <p className="text-gray-600">Просмотр статистики по врачам</p>
            </Link>
        </div>
    );

    const SeniorAdminDashboard = () => (
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
            <Link href="/senior-admin/profile" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Профиль</h3>
                <p className="text-gray-600">Управление профилем администратора</p>
            </Link>
            <Link href="/senior-admin/hospitals" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Больницы</h3>
                <p className="text-gray-600">Управление больницами</p>
            </Link>
            <Link href="/senior-admin/junior-admins" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Младшие администраторы</h3>
                <p className="text-gray-600">Управление младшими администраторами</p>
            </Link>
            <Link href="/senior-admin/users" className="bg-white p-6 rounded-lg shadow-md hover:shadow-lg transition-shadow">
                <h3 className="text-xl font-semibold text-main2 mb-2">Пользователи</h3>
                <p className="text-gray-600">Управление пользователями системы</p>
            </Link>
        </div>
    );

    // Определяем заголовок и контент в зависимости от типа пользователя
    let dashboardTitle = "Личный кабинет";
    let dashboardContent = null;

    switch (userData.userType) {
        case 'patient':
            dashboardTitle = "Кабинет пациента";
            dashboardContent = <PatientDashboard />;
            break;
        case 'doctor':
            dashboardTitle = "Кабинет врача";
            dashboardContent = <DoctorDashboard />;
            break;
        case 'junior_admin':
            dashboardTitle = "Панель младшего администратора";
            dashboardContent = <JuniorAdminDashboard />;
            break;
        case 'senior':
            dashboardTitle = "Панель старшего администратора";
            dashboardContent = <SeniorAdminDashboard />;
            break;
        default:
            dashboardContent = <p className="text-center text-gray-600">Тип пользователя не определен</p>;
    }

    return (
        <div className="max-w-6xl mx-auto py-8 px-4">
            <div className="flex flex-col md:flex-row justify-between items-start md:items-center mb-8">
                <div>
                    <h1 className="text-3xl font-bold text-main2">{dashboardTitle}</h1>
                    <p className="text-gray-600 mt-2">
                        Добро пожаловать, {userData.firstName || userData.phone}!
                    </p>
                </div>
                <button
                    onClick={handleLogout}
                    className="mt-4 md:mt-0 px-4 py-2 bg-red-500 text-white rounded hover:bg-red-600 transition-colors"
                >
                    Выйти
                </button>
            </div>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-6">
                    {error}
                </div>
            )}

            {dashboardContent}
        </div>
    );
}
